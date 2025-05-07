//  ласс дл¤ работы с последовательным портом
#pragma once
#include "TimeSynchronizer.h"
#include <qglobal.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qserialport.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtimer.h>

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
    bool init(const QString& portName, qint32 baudRate);

    QSerialPort m_serialPort;
    QString m_localBuf{ "0" };
    TimeSynchronizer* timeSynchronizer{ nullptr };
    QTimer syncTimer{ this };
    QTimer dataTimeoutTimer{ this };
};