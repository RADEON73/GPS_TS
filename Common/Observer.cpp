#include "Observer.h"
#include "Logger.h"
#include "qdir.h"

Observer::Observer(const QString& serverName, QObject* parent)
    : QObject(parent), m_serverName(serverName)
{}

Observer::~Observer()
{
    m_server.close();
    qDeleteAll(m_clients);
}

bool Observer::start()
{
    if (!m_server.listen(m_serverName)) {
        Logger::instance().error("Не удалось запустить сервер для визуализатора!");
        return false;
    }
    connect(&m_server, &QLocalServer::newConnection, this, &Observer::onNewConnection);
    Logger::instance().debug("Сервер визуализатора запущен: " + m_server.serverName() + " на " + m_server.fullServerName());
    return true;
}

void Observer::onNewConnection()
{
    QLocalSocket* client = m_server.nextPendingConnection();
    m_clients.append(client);

    connect(client, &QLocalSocket::readyRead, this, &Observer::onClientReadyRead);
    connect(client, &QLocalSocket::disconnected, this, &Observer::onClientDisconnected);
}

void Observer::onClientReadyRead() const
{
    QLocalSocket* client = qobject_cast<QLocalSocket*>(sender());
    if (!client) return;

    QString request = QString::fromUtf8(client->readAll()).trimmed();
    if (request == "GET_CONFIG_PATH") {
        QString configPath = QDir(QCoreApplication::applicationDirPath()).filePath("settings.ini");
        client->write(configPath.toUtf8());
        client->flush();
    }
}

void Observer::onClientDisconnected()
{
    QLocalSocket* client = qobject_cast<QLocalSocket*>(sender());
    if (client) {
        m_clients.removeOne(client);
        client->deleteLater();
    }
}

void Observer::sendMessage(const QString& message) const
{
    for (QLocalSocket* client : m_clients) {
        if (client->state() == QLocalSocket::ConnectedState) {
            client->write(message.toUtf8() + '\n');
            client->flush();
        }
    }
}