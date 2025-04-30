#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

class TcpServer : public QTcpServer
{
	Q_OBJECT
public:
	explicit TcpServer(QObject* parent = nullptr);
	~TcpServer() final;

protected:
	void incomingConnection(qintptr socketDescriptor) override;

private slots:
	void onReadyRead();
	void onDisconnected();

private:
	QList<QTcpSocket*> clients;
};