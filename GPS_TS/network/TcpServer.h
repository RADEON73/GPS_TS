// TcpServer.h
/*
#pragma once
#include <QTcpServer>
#include <QObject>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject* parent = nullptr);
    bool startServer(quint16 port);
    
signals:
    void dataReceived(const QByteArray& data);
    
private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();
    
private:
    QList<QTcpSocket*> m_clients;
};
*/