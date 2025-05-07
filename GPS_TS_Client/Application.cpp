#include "Application.h"
#include "Settings.h"
#include <Logger.h>

Application::Application(int& argc, char** argv) : QCoreApplication(argc, argv)
{
    setOrganizationName("RADEON");
    setApplicationName("GPS_TS_Client");

    auto& logToFile = Settings::instance().logging().logToFile;
    Logger::instance().init("", true, logToFile);

    auto& ip = Settings::instance().app().ip;
    auto& tcpPort = Settings::instance().app().port;
    client.connectToServer(ip, tcpPort);
}