#pragma once
#include <QTcpSocket>
#include <QTimer>
#include "../core/TimeSynchronizer.h"

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
    void requestData();

private:
    QTcpSocket socket{ this };
    QTimer syncTimer{ this };
    TimeSynchronizer* timeSynchronizer{ nullptr };
};