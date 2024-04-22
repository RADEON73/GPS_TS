#include "SerialPort.h"
#include <QtSerialPort/qserialportinfo.h>
#include <QtCore/qdebug.h>
#include <QtCore/qiodevice.h>
#include <iostream>
#include "../parser/NmeaParserFactory.h"
#include <variant>
#include "../core/Settings.h"
#include "../core/Logger.h"
#include <QRegularExpression>

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
    emit dataRestored();
    if (!dataTimeoutTimer.isActive())
        syncTimer.start();
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

    if (!init(portName, baudRate))
        return false;

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

bool SerialPort::init(const QString& portName, qint32 baudRate)
{
    // Проверка имени порта
    if (QRegularExpression comPortRegex("^COM\\d+$", QRegularExpression::CaseInsensitiveOption);
        !comPortRegex.match(portName).hasMatch()
        ) {
        QString error = QString("Некорректное имя порта: %1. Ожидается формат COMn").arg(portName);
        emit portError(error);
        Logger::instance().warning(error);
        return false;
    }
    m_serialPort.setPortName(portName);

    // Проверка стандартных значений BaudRate
    if (const QVector standardBaudRates = { 110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200, 128000, 256000 };
        !standardBaudRates.contains(baudRate)
        ) {
        QString warning = QString("Нестандартное значение BaudRate: %1. Возможны проблемы с работой порта").arg(baudRate);
        Logger::instance().warning(warning);
    }
    m_serialPort.setBaudRate(baudRate);

    // Получаем настройки из класса Settings
    const auto& settings = Settings::instance().serial();

    // Устанавливаем DataBits
    switch (settings.dataBits) {
    case 5: m_serialPort.setDataBits(QSerialPort::Data5); break;
    case 6: m_serialPort.setDataBits(QSerialPort::Data6); break;
    case 7: m_serialPort.setDataBits(QSerialPort::Data7); break;
    case 8: m_serialPort.setDataBits(QSerialPort::Data8); break;
    default:
        m_serialPort.setDataBits(QSerialPort::Data8);
        Logger::instance().warning("Некорректное значение DataBits, используется значение по умолчанию (8)");
        break;
    }

    // Устанавливаем Parity
    if (settings.parity == "None") {
        m_serialPort.setParity(QSerialPort::NoParity);
    }
    else if (settings.parity == "Even") {
        m_serialPort.setParity(QSerialPort::EvenParity);
    }
    else if (settings.parity == "Odd") {
        m_serialPort.setParity(QSerialPort::OddParity);
    }
    else if (settings.parity == "Space") {
        m_serialPort.setParity(QSerialPort::SpaceParity);
    }
    else if (settings.parity == "Mark") {
        m_serialPort.setParity(QSerialPort::MarkParity);
    }
    else {
        m_serialPort.setParity(QSerialPort::NoParity);
        Logger::instance().warning("Некорректное значение Parity, используется значение по умолчанию (None)");
    }

    // Устанавливаем StopBits
    if (settings.stopBits == 1) {
        m_serialPort.setStopBits(QSerialPort::OneStop);
    }
    else if (settings.stopBits == 2) {
        m_serialPort.setStopBits(QSerialPort::TwoStop);
    }
    else if (settings.stopBits == 3) {
        m_serialPort.setStopBits(QSerialPort::OneAndHalfStop);
    }
    else {
        m_serialPort.setStopBits(QSerialPort::OneStop);
        Logger::instance().warning("Некорректное значение StopBits, используется значение по умолчанию (1)");
    }

    // Устанавливаем FlowControl
    if (settings.flowControl == "None") {
        m_serialPort.setFlowControl(QSerialPort::NoFlowControl);
    }
    else if (settings.flowControl == "Hardware") {
        m_serialPort.setFlowControl(QSerialPort::HardwareControl);
    }
    else if (settings.flowControl == "Software") {
        m_serialPort.setFlowControl(QSerialPort::SoftwareControl);
    }
    else {
        m_serialPort.setFlowControl(QSerialPort::NoFlowControl);
        Logger::instance().warning("Некорректное значение FlowControl, используется значение по умолчанию (None)");
    }
    return true;
}
