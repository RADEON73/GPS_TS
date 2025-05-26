#include "MainWindow.h"
#include <qdatetime.h>
#include "../Common/Observer.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.appSelectorCb->addItem("GPS_TS_Server", ObserverServerName);
    ui.appSelectorCb->addItem("GPS_TS_Client", ObserverClientName);
}

MainWindow::~MainWindow()
{
    if (m_socket && m_socket->state() == QLocalSocket::ConnectedState) {
        m_socket->disconnectFromServer();
    }
    delete m_socket;
}

void MainWindow::on_connectBtn_clicked()
{

    if (m_socket && m_socket->state() == QLocalSocket::ConnectedState)
		disconnectFromApp();
    else
        connectToApp();
}

void MainWindow::connectToApp()
{
    if (m_socket && m_socket->state() == QLocalSocket::ConnectedState)
        return;

    ui.logDisplay->clear();

    QDateTime currentTime = QDateTime::currentDateTime();
    QString timestamp = currentTime.toString("[yyyy-MM-dd hh:mm:ss.zzz] ");

    if (!m_socket) {
        m_socket = new QLocalSocket(this);
        connect(m_socket, &QLocalSocket::readyRead, this, &MainWindow::readData);
        connect(m_socket, &QLocalSocket::errorOccurred, this, &MainWindow::handleError);
        connect(m_socket, &QLocalSocket::disconnected, this, &MainWindow::handleDisconnected);
    }

    QString appName = ui.appSelectorCb->currentData().toString();
    m_socket->connectToServer(appName);

    if (m_socket->waitForConnected(1000)) {
        ui.connectBtn->setText("Отключить");
        ui.appSelectorCb->setEnabled(false);
        appendMessage(timestamp + QString("Подключено к %1").arg(ui.appSelectorCb->currentText()));
    }
    else {
        appendMessage(timestamp + QString("Ошибка подключения к %1: %2")
            .arg(ui.appSelectorCb->currentText())
            .arg(m_socket->errorString()));
    }
}

void MainWindow::disconnectFromApp()
{
    if (m_socket && m_socket->state() == QLocalSocket::ConnectedState) {
        m_socket->disconnectFromServer();
        if (m_socket->state() == QLocalSocket::ConnectedState) {
            m_socket->waitForDisconnected(1000);
        }
    }
}

void MainWindow::readData()
{
    if (!m_socket) 
        return;
    while (m_socket->canReadLine()) {
        QByteArray dataBlock = m_socket->readLine();
        QString message = QString::fromUtf8(dataBlock).trimmed();
        appendMessage(message);
    }
}

void MainWindow::handleError(QLocalSocket::LocalSocketError socketError)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timestamp = currentTime.toString("[yyyy-MM-dd hh:mm:ss.zzz] ");

    Q_UNUSED(socketError)
        appendMessage(timestamp + QString("Ошибка соединения: %1").arg(m_socket->errorString()));
}

void MainWindow::handleDisconnected()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timestamp = currentTime.toString("[yyyy-MM-dd hh:mm:ss.zzz] ");

    appendMessage(timestamp + QString("Отключено от %1").arg(ui.appSelectorCb->currentText()));
    ui.connectBtn->setText("Подключить");
    ui.appSelectorCb->setEnabled(true);
}

void MainWindow::appendMessage(const QString& message)
{
    ui.logDisplay->appendPlainText(QString("%1").arg(message));
    ui.logDisplay->ensureCursorVisible();
}