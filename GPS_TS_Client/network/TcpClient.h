#pragma once
#include <QTcpSocket>
#include <QTimer>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject* parent = nullptr);
    ~TcpClient() final;

    void connectToServer(const QString& host, quint16 port);

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void requestData();

private:
    QTcpSocket* socket;
    QTimer* timer;
};