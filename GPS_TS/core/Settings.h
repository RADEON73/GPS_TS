#pragma once
#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT

public:
    // Группы настроек
    struct Serial
    {
        QString port{"COM1"};
        int baudRate{ 9600 };
        int dataBits{ 8 };
        QString parity{ "None" };
        int stopBits{ 1 };
        QString flowControl{ "None" };
    };

    struct Logging
    {
        QString level{ "INFO" };
        QString path{ "logs/app.log" };
        bool enabled{ false };
    };

    struct Server
    {
        QString ip;
        int port;
    };

    static Settings& instance();

    // Доступ к группам настроек
    const Serial& serial() const { return m_serial; }
    const Logging& logging() const { return m_logging; }
    const Server& server() const { return m_server; }

    // Обновление настроек
    void setSerial(const Serial& settings);
    void setLogging(const Logging& settings);
    void setServer(const Server& settings);

signals:
    void serialSettingsChanged(const Serial& newSettings);
    void loggingSettingsChanged(const Logging& newSettings);
    void serverSettingsChanged(const Server& newSettings);

private:
    explicit Settings(QObject* parent = nullptr);
    ~Settings() noexcept final = default;

    void loadSettings();
    void saveSettings();

    void initDefaultSettings();

    QSettings m_qsettings;
    Serial m_serial;
    Logging m_logging;
    Server m_server;
};