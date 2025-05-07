#pragma once
#include "SerialPort.h"
#include "TcpServer.h"
#include "TimeSynchronizer.h"
#include <qcoreapplication.h>

class Application : public QCoreApplication
{
    Q_OBJECT

public:
    Application(int& argc, char** argv);
    ~Application() final;

private:
    TimeSynchronizer synchronizer;
    SerialPort serial_port{ &synchronizer };
    TCPServer server{ &synchronizer };
};