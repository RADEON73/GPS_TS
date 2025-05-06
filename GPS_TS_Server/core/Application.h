#pragma once
#include <QtCore/QCoreApplication>
#include <QDateTime>
#include "../network/SerialPort.h"
#include "Settings.h"
#include "../network/TcpServer.h"
#include "TimeSynchronizer.h"

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