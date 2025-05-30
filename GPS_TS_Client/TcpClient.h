#pragma once
#include "TimeSynchronizer.h"
#include <qglobal.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qstring.h>
#include <qtimer.h>
#include <qabstractsocket.h>
#include <qtcpsocket.h>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(TimeSynchronizer* timeSynchronizer_, QObject* parent = nullptr);
    ~TcpClient() final;

    void connectToServer(const QString& host, quint16 port);

signals:
    void timeUpdated();

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);
    void requestData();
    void tryReconnect();

private:

    void disconnect();
    QTcpSocket socket{ this };
    QTimer syncTimer{ this };
    QTimer reconnectTimer{ this };
    TimeSynchronizer* timeSynchronizer{ nullptr };
    QString currentHost;
    quint16 currentPort{ 0 };
};