#include "TcpServer.h"
#include <QDebug>
#include <QDateTime>
#include "../core/Logger.h"

constexpr auto SYNC_TIME_TAG = "SYNC_CURRENT_TIME_RESPONSE";

TCPServer::TCPServer(TimeSynchronizer* timeSynchronizer, QObject* parent) :
	QTcpServer(parent),
	timeSynchronizer(timeSynchronizer)
{
	Logger::instance().debug("Сервер конструируется.");
}

TCPServer::~TCPServer()
{
	Logger::instance().debug("Сервер разрушается и удаляет клиентов.");
	for (auto client : clients) {
		client->disconnectFromHost();
		if (client->state() == QAbstractSocket::ConnectedState)
			client->waitForDisconnected();
		delete client;
	}
	Logger::instance().debug("Сервер закрылся.");
}

void TCPServer::incomingConnection(qintptr socketDescriptor)
{
	Logger::instance().debug("У сервера входящее соединение");
	auto client = new QTcpSocket(this);
	client->setSocketDescriptor(socketDescriptor);
	clients.append(client);

	connect(client, &QTcpSocket::readyRead, this, &TCPServer::onReadyRead);
	connect(client, &QTcpSocket::disconnected, this, &TCPServer::onDisconnected);

	Logger::instance().info("TCP клиент подключен.");
	//Logger::instance().info(QString("Новый клиент подключен:").arg(client->peerAddress().toString()));
}

void TCPServer::onReadyRead()
{
	Logger::instance().debug("Сервер инициализировал чтение данных.");
	QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
	if (!client) 
		return;
	QByteArray request = client->readAll();
	if (request == SYNC_TIME_TAG) {
		Logger::instance().debug("Получили запрос на синхронизацию времени");
		QByteArray timeData = timeSynchronizer->timeToBinary();
		client->write(timeData);
		Logger::instance().debug("Клиенту отправлено актуальное время");
	}
}

void TCPServer::onDisconnected()
{
	Logger::instance().debug("Клиент отключается от сервера.");
	QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
	if (!client) 
		return;
	clients.removeOne(client);
	client->deleteLater();
	Logger::instance().info("TCP клиент отключен.");
}