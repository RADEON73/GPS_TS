#pragma once
#include "TimeSynchronizer.h"
#include <qglobal.h>
#include <qlist.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qtcpserver.h>
#include <qtcpsocket.h>

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