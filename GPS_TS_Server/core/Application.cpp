#include "Application.h"
#include "Settings.h"
#include "Logger.h"

Application::Application(int& argc, char** argv) : QCoreApplication(argc, argv)
{
    setOrganizationName("RADEON");
    setApplicationName("GPS_TS_Server");

    auto& comPort = Settings::instance().serial().port;
    serial_port.openPort(comPort);

    auto& tcpPort = Settings::instance().app().port;
    if (!server.listen(QHostAddress::Any, tcpPort))
        Logger::instance().error("Не удалось запустить приложение!");
    Logger::instance().info(QString("Приложение запущено на порту %1").arg(tcpPort));
}

Application::~Application()
{
    serial_port.closePort(); 
}
