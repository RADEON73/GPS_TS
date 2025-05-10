#include "TcpClient.h"
#include <QDebug>
#include "../core/Logger.h"
#include "../core/Settings.h"

constexpr auto SYNC_TIME_TAG = "SYNC_CURRENT_TIME_RESPONSE";

TcpClient::TcpClient(TimeSynchronizer* timeSynchronizer, QObject* parent) :
    QObject(parent),
	timeSynchronizer(timeSynchronizer)
{
    Logger::instance().debug("Клиент конструируется!");

    connect(&socket, &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(&socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
    connect(&socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(&timer, &QTimer::timeout, this, &TcpClient::requestData);
}

TcpClient::~TcpClient()
{
    Logger::instance().debug("Клиент разрушается");
    socket.disconnectFromHost();
    if (socket.state() == QAbstractSocket::ConnectedState)
        socket.waitForDisconnected();
}

void TcpClient::connectToServer(const QString& host, quint16 port)
{
    Logger::instance().debug("Проверяем подключены ли мы к серверу, если нет то подключаемся.");
    if (socket.state() == QAbstractSocket::UnconnectedState) {
        socket.connectToHost(host, port);
        Logger::instance().info("Пытаемся подключиться к серверу!");
    }
}

void TcpClient::onConnected()
{
    Logger::instance().info("Подключено к серверу!");
    auto tInterval = Settings::instance().app().timeSyncInterval * 1000;
    timer.start(tInterval);
    requestData();
    Logger::instance().debug("Клиент запросил у сервера синхронизацию времени.");
    if (Settings::instance().app().timeSyncOn) {
        timeSynchronizer->startSync();
        Logger::instance().debug("Клиент запустил таймер синхронизации времени.");
    }

}

void TcpClient::onReadyRead()
{
    Logger::instance().debug("Клиент обрабатывает данные от сервера.");
    QByteArray timeData = socket.readAll();
    timeSynchronizer->setTimeFromBinary(timeData);
    emit timeUpdated(timeSynchronizer->currentTime());
}

void TcpClient::onDisconnected()
{
    Logger::instance().info("Отключено от сервера!");
    timer.stop();
    timeSynchronizer->stopSync();
}

void TcpClient::requestData()
{
    Logger::instance().debug("Проверяем подключены ли мы к серверу, если да то запрашиваем данные.");
    if (socket.state() == QAbstractSocket::ConnectedState) {
        socket.write(SYNC_TIME_TAG);
        Logger::instance().debug("Отправляем запрос на синхронизацию времени");
    }
}