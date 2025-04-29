#include "Settings.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>

#include <iostream>

Settings& Settings::instance()
{
    static Settings instance;
    return instance;
}

Settings::Settings(QObject* parent) : QObject(parent),
    m_qsettings(
        QDir(QCoreApplication::applicationDirPath()).filePath("settings.ini"),
        QSettings::IniFormat)
{
    initDefaultSettings();
    loadSettings();
}

void Settings::loadSettings()
{
    m_qsettings.beginGroup("Serial");
    m_serial.port = m_qsettings.value("Port", "COM1").toString();
    m_serial.baudRate = m_qsettings.value("BaudRate", 9600).toInt();
    m_serial.dataBits = m_qsettings.value("DataBits", 8).toInt();
    m_serial.parity = m_qsettings.value("Parity", "None").toString();
    m_serial.stopBits = m_qsettings.value("StopBits", 1).toInt();
    m_serial.flowControl = m_qsettings.value("FlowControl", "None").toString();
    m_qsettings.endGroup();

    m_qsettings.beginGroup("Logging");
    m_logging.level = m_qsettings.value("Level", "INFO").toString();
    m_logging.path = m_qsettings.value("Path", "logs/app.log").toString();
    m_logging.enabled = m_qsettings.value("Enabled", true).toBool();
    m_qsettings.endGroup();

    m_qsettings.beginGroup("Server");
    m_server.ip = m_qsettings.value("Ip", "127.0.0.1").toString();
    m_server.port = m_qsettings.value("Port", 2222).toInt();
    m_qsettings.endGroup();
}

void Settings::saveSettings()
{
    m_qsettings.beginGroup("Serial");
    m_qsettings.setValue("Port", m_serial.port);
    m_qsettings.setValue("BaudRate", m_serial.baudRate);
    m_qsettings.setValue("DataBits", m_serial.dataBits);
    m_qsettings.setValue("Parity", m_serial.parity);
    m_qsettings.setValue("StopBits", m_serial.stopBits);
    m_qsettings.setValue("FlowControl", m_serial.flowControl);
    m_qsettings.endGroup();

    m_qsettings.beginGroup("Logging");
    m_qsettings.setValue("Level", m_logging.level);
    m_qsettings.setValue("Path", m_logging.path);
    m_qsettings.setValue("Enabled", m_logging.enabled);
    m_qsettings.endGroup();

    m_qsettings.beginGroup("Server");
    m_qsettings.setValue("Ip", m_server.ip);
    m_qsettings.setValue("Port", m_server.port);
    m_qsettings.endGroup();

    m_qsettings.sync();
}

void Settings::setSerial(const Serial& settings)
{
    if (m_serial.port != settings.port ||
        m_serial.baudRate != settings.baudRate ||
        m_serial.dataBits != settings.dataBits ||
        m_serial.parity != settings.parity ||
        m_serial.stopBits != settings.stopBits ||
        m_serial.flowControl != settings.flowControl) {
        m_serial = settings;
        saveSettings();
        emit serialSettingsChanged(m_serial);
    }
}

void Settings::setLogging(const Logging& settings)
{
    if (m_logging.level != settings.level ||
        m_logging.path != settings.path ||
        m_logging.enabled != settings.enabled) {
        m_logging = settings;
        saveSettings();
        emit loggingSettingsChanged(m_logging);
    }
}

void Settings::setServer(const Server& settings)
{
    if (m_server.ip != settings.ip ||
        m_server.port != settings.port) {
        m_server = settings;
        saveSettings();
        emit serverSettingsChanged(m_server);
    }
}

void Settings::initDefaultSettings()
{
    // Если файл настроек пустой, инициализируем значения по умолчанию
    if (m_qsettings.allKeys().isEmpty()) {
        Serial defaultSerial;
        defaultSerial.port = "COM1";
        defaultSerial.baudRate = 9600;
        defaultSerial.dataBits = 8;
        defaultSerial.parity = "None";
        defaultSerial.stopBits = 1;
        defaultSerial.flowControl = "None";
        setSerial(defaultSerial);

        Logging defaultLogging;
        defaultLogging.level = "INFO";
        defaultLogging.path = "logs/app.log";
        defaultLogging.enabled = true;
        setLogging(defaultLogging);

        Server defaultServer;
        defaultServer.ip = "127.0.0.1";
        defaultServer.port = 2222;
        setServer(defaultServer);
    }
}