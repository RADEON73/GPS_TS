//  ласс дл¤ работы с последовательным портом
#pragma once
#include <QtCore/qobject.h>
#include <QtSerialPort/qserialport.h>
#include <QtCore/qstring.h>
#include <QtCore/qobjectdefs.h>
#include <QTimer>
#include "../core/TimeSynchronizer.h"

class SerialPort : public QObject
{
    Q_OBJECT

public:
    explicit SerialPort(TimeSynchronizer* timeSynchronizer_, QObject* parent = nullptr);

    ~SerialPort() final;

    bool openPort(const QString& portName, qint32 baudRate = QSerialPort::Baud9600);
    void closePort();
    static QStringList availablePorts();

signals:
    void portError(const QString& errorString);
    void dataTimeout();
    void dataRestored();
    void setTime(const QString& UTSDate, const QString& UTSTime);

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);
    void checkDataTimeout();

private:
    QSerialPort m_serialPort;
    QString m_localBuf{ "0" };
    TimeSynchronizer* timeSynchronizer{ nullptr };
    QTimer syncTimer{ this };
    QTimer dataTimeoutTimer{ this };
};