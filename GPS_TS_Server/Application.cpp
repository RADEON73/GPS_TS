#include "Application.h"
#include "Logger.h"
#include "SerialPort.h"
#include "Settings.h"
#include "TcpServer.h"
#include <qcoreapplication.h>
#include <qhostaddress.h>
#include <qstring.h>
#include <qlocalsocket.h>

Application::Application(int& argc, char** argv) : QCoreApplication(argc, argv)
{
    setOrganizationName("RADEON");
    setApplicationName("GPS_TS_Server");

    auto& logToFile = Settings::instance().logging().logToFile;
    Logger::instance().init("", true, logToFile);

    if (!visualizerServer.listen("GPS_TS_Visualizer"))
        Logger::instance().error("Не удалось запустить сервер для визуализатора!");
    else
        connect(&visualizerServer, &QLocalServer::newConnection, this, &Application::onNewConnection);

    auto& comPort = Settings::instance().serial().port;
    serial_port.openPort(comPort);

    auto& tcpPort = Settings::instance().app().port;
    if (!server.listen(QHostAddress::Any, tcpPort))
        Logger::instance().error("Не удалось запустить приложение!");
    Logger::instance().info(QString("Приложение запущено на порту %1").arg(tcpPort));

    connect(&serial_port, &SerialPort::dataTimeout, &server, &TCPServer::setDataNotActualState);
    connect(&serial_port, &SerialPort::dataRestored, &server, &TCPServer::setDataActualState);

    connect(&Logger::instance(), &Logger::logMessage, this, &Application::sendToVisualizer);
}

Application::~Application()
{
    visualizerServer.close();
    serial_port.closePort(); 
}

void Application::onNewConnection()
{
    QLocalSocket* client = visualizerServer.nextPendingConnection();
    visualizerClients.append(client);

    connect(client, &QLocalSocket::disconnected, [this, client]() {
        visualizerClients.removeOne(client);
        client->deleteLater();
        });
}

void Application::sendToVisualizer(const QString& message) const
{
    for (QLocalSocket* client : visualizerClients) {
        if (client->state() == QLocalSocket::ConnectedState) {
            client->write(message.toUtf8());
            client->flush();
        }
    }
}
