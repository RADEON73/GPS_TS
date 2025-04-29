#include "Application.h"
#include "Settings.h"
#include <iostream>

Application::Application(int& argc, char** argv) : QCoreApplication(argc, argv)
{
    setOrganizationName("RADEON");
    setApplicationName("GPS_TS");

    //Пример подписки на изменения состояния настроек порта
    //connect(&Settings::instance(), &Settings::serialSettingsChanged,
    //    this, [](const Settings::Serial& settings) {
    //        std::cout << "Serial settings changed! New port:" << settings.port.toStdString() << std::endl;
    //    });
    //Settings::Serial serialSettings = Settings::instance().serial();
    //serialSettings.port = "COM5"; //Времянка - переименовал порт
    //Settings::instance().setSerial(serialSettings);

    serial_port.openPort(Settings::instance().serial().port);
}

Application::~Application()
{
    serial_port.closePort(); 
}
