//  ласс дл¤ работы с последовательным портом
#pragma once
#include <QtCore/qobject.h>
#include <QtSerialPort/qserialport.h>
#include <QtCore/qstring.h>
#include <QtCore/qobjectdefs.h>

class SerialPort : public QObject
{
    Q_OBJECT

public:
    explicit SerialPort(QObject* parent = nullptr);

    ~SerialPort() final;

    bool openPort(const QString& portName, qint32 baudRate = QSerialPort::Baud9600);
    void closePort();
    static QStringList availablePorts();

signals:
    void dataReceived(const QByteArray& data);
    void portError(const QString& errorString);

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort* m_serialPort;
    QString m_localBuf{ "0" };
};