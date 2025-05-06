#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include "../core/TimeSynchronizer.h"

class TCPServer : public QTcpServer
{
	Q_OBJECT
public:
	explicit TCPServer(TimeSynchronizer* timeSynchronizer, QObject* parent = nullptr);
	~TCPServer() final;

protected:
	void incomingConnection(qintptr socketDescriptor) override;

public slots:
	void setDataActual();
	void setDataNotActual();

private slots:
	void onReadyRead();
	void onDisconnected();


private:
	QList<QTcpSocket*> clients;
	TimeSynchronizer* timeSynchronizer{ nullptr };
	bool isActual{ true }; //Флаг того, можно ли считать данные на сервере актуальными
};