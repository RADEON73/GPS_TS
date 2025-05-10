#pragma once
#include <QtCore/QCoreApplication>
#include "../network/TCPClient.h"
#include "Settings.h"
#include "TimeSynchronizer.h"

class Application : public QCoreApplication
{
    Q_OBJECT

public:
    Application(int& argc, char** argv);
    ~Application() final = default;

private:
    TimeSynchronizer synchronizer;
    TcpClient client{ &synchronizer };
};