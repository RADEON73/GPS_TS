#pragma once
#include <QtCore/QCoreApplication>
#include "../network/TCPClient.h"
#include "Settings.h"

class Application : public QCoreApplication
{
    Q_OBJECT

public:
    Application(int& argc, char** argv);
    ~Application() final;

private:
    TcpClient client;
};