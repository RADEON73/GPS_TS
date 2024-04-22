#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include "../core/TimeSynchronizer.h"

class TCPServer : public QTcpServer
{
	Q_OBJECT

	enum class ServerDataState
	{
		ACTUAL,
		NOT_ACTUAL,
		UNKNOWN
	};

public:
	explicit TCPServer(TimeSynchronizer* timeSynchronizer, QObject* parent = nullptr);
	~TCPServer() final;

protected:
	void incomingConnection(qintptr socketDescriptor) override;

public slots:
	void setDataActualState() { dataState = ServerDataState::ACTUAL; };
	void setDataNotActualState() { dataState = ServerDataState::NOT_ACTUAL; };

private slots:
	void onReadyRead();
	void onDisconnected();


private:
	QList<QTcpSocket*> clients;
	TimeSynchronizer* timeSynchronizer{ nullptr };
	ServerDataState dataState{ ServerDataState::UNKNOWN };
};