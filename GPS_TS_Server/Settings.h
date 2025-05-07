#pragma once
#include <qobject.h>
#include <qobjectdefs.h>
#include <qsettings.h>
#include <qstring.h>

class Settings : public QObject
{
    Q_OBJECT

public:
    // Группы настроек
    struct Serial
    {
        QString port;
        int baudRate;
        int dataBits;
        QString parity;
        int stopBits;
        QString flowControl;
    };

    struct Logging
    {
        bool logRMC;
        bool logGGA;
		bool logGLL;
		bool logGSA;
        bool logGSV;
        bool logVTG;
        bool logPMTK;
        bool logToFile;
        QString path;
    };

    struct App
    {
        bool timeSyncOn;
        int timeSyncInterval;
        int port;
    };

    static Settings& instance();

    // Доступ к группам настроек
    const Serial& serial() const { return m_serial; }
    const Logging& logging() const { return m_logging; }
    const App& app() const { return m_app; }

    // Обновление настроек
    void setSerial(const Serial& settings);
    void setLogging(const Logging& settings);
    void appServer(const App& settings);

signals:
    void serialSettingsChanged(const Serial& newSettings);
    void loggingSettingsChanged(const Logging& newSettings);
    void appSettingsChanged(const App& newSettings);

private:
    explicit Settings(QObject* parent = nullptr);
    ~Settings() noexcept final = default;

    void Init();
    void loadSettings();
    void saveSettings();

    QSettings m_qsettings;
    Serial m_serial;
    Logging m_logging;
    App m_app;
};