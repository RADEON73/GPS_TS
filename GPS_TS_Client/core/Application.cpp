#include "Application.h"
#include "Settings.h"
#include <iostream>

Application::Application(int& argc, char** argv) : QCoreApplication(argc, argv)
{
    setOrganizationName("RADEON");
    setApplicationName("GPS_TS_Client");

    client.connectToServer("127.0.0.1", 2222);
}

Application::~Application()
{
}
