#pragma once
#include "TCPClient.h"
#include "Observer.h"
#include "TimeSynchronizer.h"
#include <qcoreapplication.h>

class Application : public QCoreApplication
{
    Q_OBJECT

public:
    Application(int& argc, char** argv);
    ~Application() final = default;

private:
    TimeSynchronizer synchronizer;
    TcpClient tcpClient{ &synchronizer };
    Observer observer{ ObserverClientName, this };
};