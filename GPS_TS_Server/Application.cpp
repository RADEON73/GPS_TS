#include "Application.h"
#include "Logger.h"
#include "SerialPort.h"
#include "Settings.h"
#include "TcpServer.h"
#include <qcoreapplication.h>
#include <qhostaddress.h>
#include <qstring.h>

Application::Application(int& argc, char** argv) : QCoreApplication(argc, argv)
{
    setOrganizationName("RADEON");
    setApplicationName("GPS_TS_Server");

    auto& logToFile = Settings::instance().logging().logToFile;
    Logger::instance().init("", true, logToFile);

    observer.start();

    auto& comPort = Settings::instance().serial().port;
    serialPort.openPort(comPort);

    auto& tcpPort = Settings::instance().app().port;
    if (!tcpServer.listen(QHostAddress::Any, tcpPort))
        Logger::instance().error("Не удалось запустить приложение!");
    Logger::instance().info(QString("Приложение запущено на порту %1").arg(tcpPort));

    connect(&serialPort, &SerialPort::dataTimeout, &tcpServer, &TCPServer::setDataNotActualState);
    connect(&serialPort, &SerialPort::dataRestored, &tcpServer, &TCPServer::setDataActualState);

    connect(&Logger::instance(), &Logger::logMessage, &observer, &Observer::sendMessage);
}

Application::~Application()
{
    serialPort.closePort();
}