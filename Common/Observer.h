#pragma once
#include <qlocalserver.h>
#include <qlocalsocket.h>
#include <qobject.h>

inline const QString ObserverServerName = "GPS_TS_Server_Observer";
inline const QString ObserverClientName = "GPS_TS_Client_Observer";

class Observer : public QObject
{
    Q_OBJECT

public:
    explicit Observer(const QString& serverName, QObject* parent = nullptr);
    ~Observer() override;

    bool start();
    void sendMessage(const QString& message) const;

private slots:
    void onNewConnection();
    void onClientDisconnected();

private:
    QLocalServer m_server;
    QList<QLocalSocket*> m_clients;
    QString m_serverName;
};