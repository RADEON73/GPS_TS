#include "SerialPort.h"
#include <QtSerialPort/qserialportinfo.h>
#include <QtCore/qdebug.h>
#include <QtCore/qiodevice.h>
#include <iostream>
#include "../parser/NmeaParserFactory.h"
#include <variant>
#include "../core/Settings.h"

SerialPort::SerialPort(TimeSynchronizer* timeSynchronizer_, QObject* parent) :
    QObject(parent),
    timeSynchronizer(timeSynchronizer_)
{
    m_serialPort = new QSerialPort(this);

    // Подключаем сигналы
    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPort::handleReadyRead);
    connect(m_serialPort, &QSerialPort::errorOccurred, this, &SerialPort::handleError);

    connect(this, &SerialPort::setTime, timeSynchronizer, &TimeSynchronizer::setTime);
    connect(&syncTimer, &QTimer::timeout, timeSynchronizer, &TimeSynchronizer::synchronizeTime);
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
                    auto data = nmeaParser->parse(m_localBuf.split(','));
                    if (auto rmcData = std::get_if<RMC_Data>(&data)) {
                        emit setTime(rmcData->Date, rmcData->UTC_Time);
                    }
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

    auto tInterval = Settings::instance().app().timeSyncInterval * 1000;
    syncTimer.start(tInterval);

    return true;
}

void SerialPort::closePort()
{
    syncTimer.stop();
    if (m_serialPort->isOpen())
        m_serialPort->close();
}