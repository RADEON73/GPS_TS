#pragma once
#include <qobject.h>
#include <qobjectdefs.h>
#include <qsettings.h>
#include <qstring.h>

class Settings : public QObject
{
    Q_OBJECT

public:

    struct Logging
    {
        bool logToFile;
        QString path;
    };

    struct App
    {
        bool timeSyncOn;
        int timeSyncInterval;
        QString ip;
        int port;
    };

    static Settings& instance();

    // Доступ к группам настроек
    const Logging& logging() const { return m_logging; }
    const App& app() const { return m_app; }

    // Обновление настроек
    void setLogging(const Logging& settings);
    void appServer(const App& settings);

signals:
    void loggingSettingsChanged(const Logging& newSettings);
    void appSettingsChanged(const App& newSettings);

private:
    explicit Settings(QObject* parent = nullptr);
    ~Settings() noexcept final = default;

    void Init();
    void loadSettings();
    void saveSettings();

    QSettings m_qsettings;
    Logging m_logging;
    App m_app;
};