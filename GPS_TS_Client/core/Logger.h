#pragma once
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>

class Logger : public QObject
{
    Q_OBJECT

public:
    enum class LogLevel
    {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    static Logger& instance();

    void init(const QString& logFilePath = "", bool consoleOutput = true);
    void log(LogLevel level, const QString& message, const QString& category = "");
    void setLoggingEnabled(bool enabled);

    // Удобные методы для быстрого логирования
    void debug(const QString& message, const QString& category = "");
    void info(const QString& message, const QString& category = "");
    void warning(const QString& message, const QString& category = "");
    void error(const QString& message, const QString& category = "");
    void critical(const QString& message, const QString& category = "");

signals:
    void logMessage(const QString& formattedMessage);

private:
    explicit Logger(QObject* parent = nullptr);
    ~Logger() override;

    QString levelToString(LogLevel level) const;
    QString formatMessage(LogLevel level, const QString& message, const QString& category) const;

    QFile m_logFile;
    QTextStream m_fileStream;
    bool m_consoleOutput;
    bool m_enabled;
    mutable QMutex m_mutex; // Для потокобезопасности
};