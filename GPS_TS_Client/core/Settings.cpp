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
    Init();
}

void Settings::Init()
{
    loadSettings();
    saveSettings();
}

void Settings::loadSettings()
{
    m_qsettings.beginGroup("Logging");
    m_logging.logToFile = m_qsettings.value("Enabled", false).toBool();
    m_logging.path = m_qsettings.value("Path", "logs/app.log").toString();
    m_qsettings.endGroup();

    m_qsettings.beginGroup("TcpServer");
    m_app.ip = m_qsettings.value("Ip", "127.0.0.1").toString();
    m_app.port = m_qsettings.value("Port", 2222).toInt();
    m_qsettings.endGroup();
}

void Settings::saveSettings()
{
    m_qsettings.beginGroup("Logging");
    m_qsettings.setValue("logToFile", m_logging.logToFile);
    m_qsettings.setValue("Path", m_logging.path);
    m_qsettings.endGroup();

    m_qsettings.beginGroup("TcpServer");
    m_qsettings.setValue("Ip", m_app.ip);
    m_qsettings.setValue("Port", m_app.port);
    m_qsettings.endGroup();

    m_qsettings.sync();
}

void Settings::setLogging(const Logging& settings)
{
    if (m_logging.logToFile != settings.logToFile ||
        m_logging.path != settings.path) {
        m_logging = settings;
        saveSettings();
        emit loggingSettingsChanged(m_logging);
    }
}

void Settings::appServer(const TcpServer& settings)
{
    if (m_app.ip != settings.ip ||
        m_app.port != settings.port) {
        m_app = settings;
        saveSettings();
        emit appSettingsChanged(m_app);
    }
}