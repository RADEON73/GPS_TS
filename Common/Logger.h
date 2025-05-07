#pragma once
#include <qfile.h>
#include <qmutex.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qstring.h>
#include <qtextstream.h>
#include <qcoreapplication.h>
#include <qdir.h>

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

    void init(const QString& logFilePath = "", bool consoleOutput = true, bool enableFileLogging = false);
    void log(LogLevel level, const QString& message, const QString& category = "");
    void setLoggingEnabled(bool enabled);
    void setFileLoggingEnabled(bool enabled);

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

    void createLogDirectory() const;
    void openLogFile();

    QFile m_logFile;
    QTextStream m_fileStream;
    bool m_consoleOutput{ true };
    bool m_enabled{ true };
    bool m_fileLoggingEnabled{ false };
    QString m_logDirectory{ QCoreApplication::applicationDirPath() + "/Log" };
    mutable QMutex m_mutex; // Для потокобезопасности
};