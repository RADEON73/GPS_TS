#include "Application.h"
#include "Settings.h"

Application::Application(int& argc, char** argv) : QCoreApplication(argc, argv)
{
    setOrganizationName("RADEON");
    setApplicationName("GPS_TS_Client");
    auto& ip = Settings::instance().app().ip;
    auto& tcpPort = Settings::instance().app().port;
    client.connectToServer(ip, tcpPort);
}