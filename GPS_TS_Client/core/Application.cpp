#include "Application.h"
#include "Settings.h"
#include <iostream>

Application::Application(int& argc, char** argv) : QCoreApplication(argc, argv)
{
    setOrganizationName("RADEON");
    setApplicationName("GPS_TS_Client");
    auto& ip = Settings::instance().app().ip;
    auto& tcpPort = Settings::instance().app().port;
    client.connectToServer(ip, tcpPort);

    synchronizer.setSyncInterval(Settings::instance().app().timeSyncInterval);
}