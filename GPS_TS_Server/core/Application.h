#pragma once
#include <QtCore/QCoreApplication>
#include "../network/SerialPort.h"
#include "Settings.h"
#include "../network/TcpServer.h"

class Application : public QCoreApplication
{
    Q_OBJECT

public:
    Application(int& argc, char** argv);
    ~Application() final;

private:
    SerialPort serial_port;
    TcpServer app;
};