#include "TcpClient.h"
#include <QDebug>
#include "../core/Logger.h"

TcpClient::TcpClient(QObject* parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    timer = new QTimer(this);

    connect(socket, &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(timer, &QTimer::timeout, this, &TcpClient::requestData);
}

TcpClient::~TcpClient()
{
    socket->disconnectFromHost();
    if (socket->state() == QAbstractSocket::ConnectedState)
        socket->waitForDisconnected();
    delete socket;
}

void TcpClient::connectToServer(const QString& host, quint16 port)
{
    if (socket->state() == QAbstractSocket::UnconnectedState) {
        socket->connectToHost(host, port);
        Logger::instance().info("Try connecting to TcpServer!");
    }
}

void TcpClient::onConnected()
{
    Logger::instance().info("Connected to TcpServer!");
    timer->start(3000);  // «апрос данных каждые 3 секунды
}

void TcpClient::onReadyRead()
{
    QString response = QString::fromUtf8(socket->readAll());
    Logger::instance().info(QString("Received data from TcpServer:").arg(response));
    // «десь можно обработать данные (например, сохранить или вывести)
}

void TcpClient::onDisconnected()
{
    Logger::instance().info("Disconnected from TcpServer!");
    timer->stop();
}

void TcpClient::requestData()
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write("SYNC_CURRENT_TIME_RESPONSE");
    }
}