#include "SerialPort.h"
#include <QtSerialPort/qserialportinfo.h>
#include <QtCore/qdebug.h>
#include <QtCore/qiodevice.h>
#include <iostream>
#include "../parser/NmeaParserFactory.h"
#include <variant>
#include "../core/Settings.h"
#include "../core/Logger.h"

constexpr int DATA_LOST_INTERVAL_MS = 5000;

SerialPort::SerialPort(TimeSynchronizer* timeSynchronizer_, QObject* parent) :
    QObject(parent),
    timeSynchronizer(timeSynchronizer_)
{
    dataTimeoutTimer.setInterval(DATA_LOST_INTERVAL_MS);
    dataTimeoutTimer.setSingleShot(true);
    connect(&dataTimeoutTimer, &QTimer::timeout, this, &SerialPort::checkDataTimeout);

    connect(&m_serialPort, &QSerialPort::readyRead, this, &SerialPort::handleReadyRead);
    connect(&m_serialPort, &QSerialPort::errorOccurred, this, &SerialPort::handleError);
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
    if (!dataTimeoutTimer.isActive()) {
        emit dataRestored();
        syncTimer.start();
    }
    dataTimeoutTimer.start();

    for (auto i : m_serialPort.readAll()) {
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
    if (error == QSerialPort::NoError)
        return;

    QString errorString = m_serialPort.errorString();
    emit portError(errorString);

    switch (error) {
    case QSerialPort::ResourceError:
        Logger::instance().warning("Ошибка ресурсов - соединение закрыто:" + errorString);
        break;
    case QSerialPort::PermissionError:
        Logger::instance().warning("Ошибка прав доступа - соединение закрыто:" + errorString);
        break;
    case QSerialPort::DeviceNotFoundError:
        Logger::instance().warning("Устройство не найдено - соединение закрыто:" + errorString);
        break;
    default:
        Logger::instance().warning("Ошибка серийного порта" + m_serialPort.portName() + ":" + errorString);
        break;
    }
    if (m_serialPort.isOpen())
        closePort();
}

bool SerialPort::openPort(const QString& portName, qint32 baudRate)
{
    if (m_serialPort.isOpen())
        closePort();

    m_serialPort.setPortName(portName);
    m_serialPort.setBaudRate(baudRate);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setParity(QSerialPort::NoParity);
    m_serialPort.setStopBits(QSerialPort::OneStop);
    m_serialPort.setFlowControl(QSerialPort::NoFlowControl);

    if (!m_serialPort.open(QIODevice::ReadWrite)) {
        QString errorString = m_serialPort.errorString();
        emit portError(errorString);
        Logger::instance().warning("Ошибка при открытии порта" + portName + ":" + errorString);
        return false;
    }

    dataTimeoutTimer.start();
    Logger::instance().info("Порт " + m_serialPort.portName() + " успешно открыт");

    if (Settings::instance().app().timeSyncOn) {
        auto tInterval = Settings::instance().app().timeSyncInterval * 1000;
        syncTimer.start(tInterval);
    }
    return true;
}

void SerialPort::closePort()
{
    syncTimer.stop();
    dataTimeoutTimer.stop();
    if (m_serialPort.isOpen()) {
        m_serialPort.close();
        Logger::instance().info("Порт " + m_serialPort.portName() + "закрыт");
    }

}

void SerialPort::checkDataTimeout()
{
    Logger::instance().warning(m_serialPort.portName() + ": Прекращение поступления данных, ожидаем восстановление соединения...");
    syncTimer.stop();
    emit dataTimeout();
}