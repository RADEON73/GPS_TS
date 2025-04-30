#include "Application.h"
#include "Settings.h"
#include <iostream>
#include "Logger.h"


Application::Application(int& argc, char** argv) : QCoreApplication(argc, argv)
{
    setOrganizationName("RADEON");
    setApplicationName("GPS_TS_Server");

    auto& comPort = Settings::instance().serial().port;
    serial_port.openPort(comPort);

    auto& tcpPort = Settings::instance().app().port;
    if (!app.listen(QHostAddress::Any, tcpPort))
        Logger::instance().error("TcpServer could not start!");
    Logger::instance().info(QString("TcpServer started on port %1").arg(tcpPort));
}

Application::~Application()
{
    serial_port.closePort(); 
}
