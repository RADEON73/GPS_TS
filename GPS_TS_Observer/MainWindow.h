#pragma once
#include <qmainwindow.h>
#include "ui_MainWindow.h"
#include "qlocalsocket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() final;


private slots:
    void on_connectBtn_clicked();
    void on_openConfigBtn_clicked();
    void readData();
    void handleError(QLocalSocket::LocalSocketError socketError);
    void handleDisconnected();

private:
    void appendMessage(const QString& message);
    void connectToApp();
    void disconnectFromApp();
    void requestConfigPath();

    Ui::MainWindow ui;
    QLocalSocket* m_socket{ nullptr };
};