#pragma once
#include "SerialPort.h"
#include "TcpServer.h"
#include "TimeSynchronizer.h"
#include <qcoreapplication.h>
#include <qlocalserver.h>

class Application : public QCoreApplication
{
    Q_OBJECT

public:
    Application(int& argc, char** argv);
    ~Application() final;

private slots:
    void onNewConnection();  // ���������� ����� ����������� �������������
    void sendToVisualizer(const QString& message) const;  // �������� ������ � ������������

private:
    TimeSynchronizer synchronizer;
    SerialPort serial_port{ &synchronizer };
    TCPServer server{ &synchronizer };
    QLocalServer visualizerServer;  // ������ ��� �������������
    QList<QLocalSocket*> visualizerClients;  // ������ ������������ ��������������
};