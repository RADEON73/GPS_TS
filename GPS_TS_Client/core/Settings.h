#pragma once
#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT

public:

    struct Logging
    {
        bool logToFile;
        QString path;
    };

    struct TcpServer
    {
        QString ip;
        int port;
    };

    static Settings& instance();

    // Доступ к группам настроек
    const Logging& logging() const { return m_logging; }
    const TcpServer& app() const { return m_app; }

    // Обновление настроек
    void setLogging(const Logging& settings);
    void appServer(const TcpServer& settings);

signals:
    void loggingSettingsChanged(const Logging& newSettings);
    void appSettingsChanged(const TcpServer& newSettings);

private:
    explicit Settings(QObject* parent = nullptr);
    ~Settings() noexcept final = default;

    void Init();
    void loadSettings();
    void saveSettings();

    QSettings m_qsettings;
    Logging m_logging;
    TcpServer m_app;
};