#include "SerialPort.h"
#include <QtSerialPort/qserialportinfo.h>
#include <QtCore/qdebug.h>
#include <QtCore/qiodevice.h>
#include <iostream>
#include "../parser/NmeaParserFactory.h"

SerialPort::SerialPort(QObject* parent) : QObject(parent)
{
    m_serialPort = new QSerialPort(this);

    // Подключаем сигналы
    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPort::handleReadyRead);
    connect(m_serialPort, &QSerialPort::errorOccurred, this, &SerialPort::handleError);
}

SerialPort::~SerialPort()
{
    closePort();
}

QStringList SerialPort::availablePorts()
{
    QStringList ports;
    foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
        ports << info.portName();
    return ports;
}

void SerialPort::handleReadyRead()
{
    QByteArray gpsPacket = m_serialPort->readAll();
    emit dataReceived(gpsPacket);

    for (auto i : gpsPacket) {
        if (i == '$') {
            m_localBuf.remove('\n');
            m_localBuf.remove('\r');
            if (m_localBuf[0] == '$') {
                if (auto nmeaParser = NmeaParserFactory::createParser(m_localBuf)) {
                    nmeaParser->parse(m_localBuf.split(','));
                    nmeaParser->process();
                }
            }
            m_localBuf.clear();
        }
        m_localBuf.append(i);
    }
}

void SerialPort::handleError(QSerialPort::SerialPortError error)
{
    emit portError(m_serialPort->errorString());
    switch (error) {
    case QSerialPort::ResourceError:
        if (m_serialPort->isOpen())
            m_serialPort->close();
        qWarning() << "Serial port ResourceError - connection closed:" << m_serialPort->errorString();
        break;
    case QSerialPort::NoError:
        qInfo() << "Port" << m_serialPort->portName() << "opened successfully";
        break;
    default:
        qWarning() << "Failed to open port"  << m_serialPort->portName() << ":" << m_serialPort->errorString();
        break;
    }

}

bool SerialPort::openPort(const QString& portName, qint32 baudRate)
{
    if (m_serialPort->isOpen())
        closePort();

    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(baudRate);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!m_serialPort->open(QIODevice::ReadWrite))
        return false;

    return true;
}

void SerialPort::closePort()
{
    if (m_serialPort->isOpen())
        m_serialPort->close();
}