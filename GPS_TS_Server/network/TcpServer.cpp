#include "TcpServer.h"
#include <QDebug>
#include <QDateTime>
#include "../core/Logger.h"

TcpServer::TcpServer(QObject* parent) : QTcpServer(parent) {}

TcpServer::~TcpServer()
{
	for (auto client : clients) {
		client->disconnectFromHost();
		if (client->state() == QAbstractSocket::ConnectedState)
			client->waitForDisconnected();
		delete client;
	}
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	auto client = new QTcpSocket(this);
	client->setSocketDescriptor(socketDescriptor);
	clients.append(client);

	connect(client, &QTcpSocket::readyRead, this, &TcpServer::onReadyRead);
	connect(client, &QTcpSocket::disconnected, this, &TcpServer::onDisconnected);

	Logger::instance().info(QString("New client connected:").arg(client->peerAddress().toString()));
}

void TcpServer::onReadyRead()
{
	QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
	if (!client) return;

	QString request = QString::fromUtf8(client->readAll());

	Logger::instance().info(QString("Received request from client:").arg(request));

	if (request.trimmed() == "SYNC_CURRENT_TIME_RESPONSE") {

		QString response = "Hello from app! Current time: " + QDateTime::currentDateTime().toString();
		client->write(response.toUtf8());
	}
}

void TcpServer::onDisconnected()
{
	QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
	if (!client) return;

	clients.removeOne(client);
	client->deleteLater();

	Logger::instance().info("TcpClient disconnected");
}