#include "Settings.h"
#include <qcoreapplication.h>
#include <qdir.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qsettings.h>

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
    m_qsettings.beginGroup("Serial");
    m_serial.port = m_qsettings.value("Port", "COM1").toString();
    m_serial.baudRate = m_qsettings.value("BaudRate", 9600).toInt();
    m_serial.dataBits = m_qsettings.value("DataBits", 8).toInt();
    m_serial.parity = m_qsettings.value("Parity", "None").toString();
    m_serial.stopBits = m_qsettings.value("StopBits", 1).toInt();
    m_serial.flowControl = m_qsettings.value("FlowControl", "None").toString();
    m_qsettings.endGroup();

    m_qsettings.beginGroup("Logging");
    m_logging.logRMC = m_qsettings.value("logRMC", true).toBool();
    m_logging.logGGA = m_qsettings.value("logGGA", false).toBool();
    m_logging.logGLL = m_qsettings.value("logGLL", false).toBool();
    m_logging.logGSA = m_qsettings.value("logGSA", false).toBool();
    m_logging.logGSV = m_qsettings.value("logGSV", false).toBool();
    m_logging.logVTG = m_qsettings.value("logVTG", false).toBool();
    m_logging.logPMTK = m_qsettings.value("logPMTK", false).toBool();
    m_logging.logToFile = m_qsettings.value("Enabled", false).toBool();
    m_logging.path = m_qsettings.value("Path", "logs/app.log").toString();
    m_qsettings.endGroup();

    m_qsettings.beginGroup("App");
    m_app.timeSyncOn = m_qsettings.value("TimeSyncOn", true).toBool();
	m_app.timeSyncInterval = m_qsettings.value("TimeSyncInterval", 10).toInt();
    m_app.port = m_qsettings.value("Port", 2222).toInt();
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
    m_qsettings.setValue("logRMC", m_logging.logRMC);
    m_qsettings.setValue("logGGA", m_logging.logGGA);
    m_qsettings.setValue("logGLL", m_logging.logGLL);
    m_qsettings.setValue("logGSA", m_logging.logGSA);
    m_qsettings.setValue("logGSV", m_logging.logGSV);
    m_qsettings.setValue("logVTG", m_logging.logVTG);
    m_qsettings.setValue("logPMTK", m_logging.logPMTK);
    m_qsettings.setValue("logToFile", m_logging.logToFile);
    m_qsettings.setValue("Path", m_logging.path);
    m_qsettings.endGroup();

    m_qsettings.beginGroup("App");
    m_qsettings.setValue("TimeSyncOn", m_app.timeSyncOn);
    m_qsettings.setValue("TimeSyncInterval", m_app.timeSyncInterval);
    m_qsettings.setValue("Port", m_app.port);
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
    if (m_logging.logRMC != settings.logRMC ||
        m_logging.logGGA != settings.logGGA ||
        m_logging.logGLL != settings.logGLL ||
        m_logging.logGSA != settings.logGSA ||
        m_logging.logGSV != settings.logGSV ||
        m_logging.logVTG != settings.logVTG ||
        m_logging.logPMTK != settings.logPMTK ||
        m_logging.logToFile != settings.logToFile ||
        m_logging.path != settings.path) {
        m_logging = settings;
        saveSettings();
        emit loggingSettingsChanged(m_logging);
    }
}

void Settings::appServer(const App& settings)
{
    if (m_app.timeSyncOn != settings.timeSyncOn ||
        m_app.timeSyncInterval != settings.timeSyncInterval ||
        m_app.port != settings.port) {
        m_app = settings;
        saveSettings();
        emit appSettingsChanged(m_app);
    }
}