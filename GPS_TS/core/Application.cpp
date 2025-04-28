#include "Application.h"

Application::Application(int& argc, char** argv) : QCoreApplication(argc, argv)
{
    setOrganizationName("RADEON");
    setApplicationName("GPS_TS");
    serial_port.openPort("COM4");
}

Application::~Application()
{
    serial_port.closePort(); 
}
