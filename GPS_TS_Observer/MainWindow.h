#pragma once
#include <QMainWindow>
#include "ui_MainWindow.h"
#include "Connect.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() final = default;

private:
    Connect connect_ui{this};
};