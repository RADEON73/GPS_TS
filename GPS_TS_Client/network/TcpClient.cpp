#include "TcpClient.h"
#include <QDebug>
#include "../core/Logger.h"
#include "../core/Settings.h"
#include <QJsonObject>
#include <QJsonDocument>

constexpr auto SYNC_TIME_TAG = "SYNC_CURRENT_TIME_RESPONSE";
constexpr int RECONNECT_INTERVAL_MS = 5000; // 5 seconds between stop attempts

TcpClient::TcpClient(TimeSynchronizer* timeSynchronizer_, QObject* parent) :
    QObject(parent),
	timeSynchronizer(timeSynchronizer_)
{
    Logger::instance().debug("Клиент конструируется!");

    connect(&socket, &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(&socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
    connect(&socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(&socket, &QTcpSocket::errorOccurred, this, &TcpClient::onError);
    connect(&syncTimer, &QTimer::timeout, this, &TcpClient::requestData);
    connect(&reconnectTimer, &QTimer::timeout, this, &TcpClient::tryReconnect);
    connect(this, &TcpClient::timeUpdated, timeSynchronizer, &TimeSynchronizer::synchronizeTime);
}

TcpClient::~TcpClient()
{
    syncTimer.stop();
    reconnectTimer.stop();
    Logger::instance().debug("Клиент разрушается");
    socket.disconnectFromHost();
    if (socket.state() == QAbstractSocket::ConnectedState)
        socket.waitForDisconnected();
}

void TcpClient::connectToServer(const QString& host, quint16 port)
{
    currentHost = host;
    currentPort = port;

    Logger::instance().debug("Проверяем подключены ли мы к серверу, если нет то подключаемся.");
    if (socket.state() == QAbstractSocket::UnconnectedState) {
        socket.connectToHost(host, port);
        Logger::instance().info("Попытка подключения к серверу...");
    }
}

void TcpClient::onConnected()
{
    Logger::instance().info("Подключено к серверу!");
    auto tInterval = Settings::instance().app().timeSyncInterval * 1000;
    Logger::instance().debug("Клиент запросил у сервера синхронизацию времени.");
    if (Settings::instance().app().timeSyncOn) {
        syncTimer.start(tInterval);
        requestData();
        Logger::instance().debug("Клиент запустил таймер синхронизации времени.");
    }
    reconnectTimer.stop();
}

void TcpClient::onReadyRead()
{
    Logger::instance().debug("Клиент обрабатывает данные от сервера.");
    QByteArray data = socket.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        Logger::instance().warning("Неверный формат ответа от сервера");
        return;
    }
    QJsonObject response = doc.object();
    QString status = response["status"].toString();

    if (status == "error") {
        QString errorMessage = response["message"].toString();
        Logger::instance().warning("Ошибка от сервера: " + errorMessage);
    }
    else if (status == "success") {
        QByteArray timeData = QByteArray::fromBase64(response["timeData"].toString().toUtf8());
        timeSynchronizer->timeFromBinary(timeData);
        emit timeUpdated();
    }
}

void TcpClient::requestData()
{
    Logger::instance().debug("Проверяем подключены ли мы к серверу, если да то запрашиваем данные.");
    if (socket.state() == QAbstractSocket::ConnectedState) {
        socket.write(SYNC_TIME_TAG);
        Logger::instance().debug("Отправляем запрос на синхронизацию времени");
    }
}

void TcpClient::onDisconnected()
{
    disconnect();
}

void TcpClient::onError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
        Logger::instance().warning("Connection error: " + socket.errorString());
    disconnect();
    socket.abort();
}

void TcpClient::tryReconnect()
{
    Logger::instance().info("Попытка подключения к серверу...");
    socket.connectToHost(currentHost, currentPort);
}

void TcpClient::disconnect()
{
    if (!reconnectTimer.isActive()) {
        syncTimer.stop();
        reconnectTimer.start(RECONNECT_INTERVAL_MS);
        Logger::instance().info("Пытаемся подключиться к серверу каждые " +
            QString::number(RECONNECT_INTERVAL_MS / 1000) + " секунд");
    }
}
