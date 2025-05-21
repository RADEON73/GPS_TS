#pragma once
#include "Logger.h"
#include <qdatetime.h>
#include <qdebug.h>
#include <qiodevice.h>
#include <qlogging.h>
#include <qmutex.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qstring.h>
#include <qtextstream.h>
#include <qdir.h>

Logger& Logger::instance()
{
    static Logger instance;
    return instance;
}

Logger::Logger(QObject* parent) : QObject(parent)
{
}

Logger::~Logger()
{
    if (m_logFile.isOpen())
        m_logFile.close();
}

void Logger::init(const QString& logFilePath, bool consoleOutput, bool enableFileLogging)
{
    QMutexLocker locker(&m_mutex);

    m_consoleOutput = consoleOutput;
    m_fileLoggingEnabled = enableFileLogging;

    if (!logFilePath.isEmpty()) {
        m_logFile.setFileName(logFilePath);
        if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            emit logMessage(formatMessage(LogLevel::Error,
                QString("Failed to open log file: %1").arg(logFilePath), "Logger"));
            return;
        }
        m_fileStream.setDevice(&m_logFile);
    }
}

void Logger::log(LogLevel level, const QString& message, const QString& category)
{
    if (!m_enabled) 
        return;

    QMutexLocker locker(&m_mutex);

    const QString formatted = formatMessage(level, message, category);

    if (m_consoleOutput)
        qDebug().noquote() << formatted;

    if (m_fileLoggingEnabled) {
        openLogFile();
        if (m_logFile.isOpen()) {
            m_fileStream << formatted << Qt::endl;
            m_fileStream.flush();
        }
    }

    emit logMessage(formatted);
}

QString Logger::levelToString(LogLevel level) const
{
    switch (level) {
    case LogLevel::Debug:    return "DEBUG";
    case LogLevel::Info:     return "INFO";
    case LogLevel::Warning:  return "WARNING";
    case LogLevel::Error:    return "ERROR";
    case LogLevel::Critical: return "CRITICAL";
    default:                 return "UNKNOWN";
    }
}

QString Logger::formatMessage(LogLevel level, const QString& message, const QString& category) const
{
    QString formatted = QString("[%1] [%2]")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
        .arg(levelToString(level));

    if (!category.isEmpty())
        formatted += QString(" [%1]").arg(category);

    formatted += QString(": %1").arg(message);

    return formatted;
}

void Logger::createLogDirectory() const
{
    QDir logDir(m_logDirectory);
    if (!logDir.exists())
        logDir.mkpath(".");
    //TODO : нужна обработка ошибок при создании директории!
}

void Logger::openLogFile()
{
    createLogDirectory();

    QString appName = QCoreApplication::applicationName();
    if (appName.isEmpty())
        appName = "application";

    QString dateStr = QDateTime::currentDateTime().toString("yyyyMMdd");
    QString fileName = QString("%1/%2_%3.log").arg(m_logDirectory).arg(appName).arg(dateStr);

    if (m_logFile.isOpen()) {
        // Если файл уже открыт и это тот же файл, ничего не делаем
        if (m_logFile.fileName() == fileName)
            return;
        // Если это другой файл, закрываем текущий
        m_logFile.close();
    }
    m_logFile.setFileName(fileName);
    if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        emit logMessage(formatMessage(LogLevel::Error,
            QString("Failed to open log file: %1").arg(fileName), "Logger"));
        return;
    }
    m_fileStream.setDevice(&m_logFile);
}

void Logger::setFileLoggingEnabled(bool enabled)
{
    QMutexLocker locker(&m_mutex);
    m_fileLoggingEnabled = enabled;

    if (!enabled && m_logFile.isOpen())
        m_logFile.close();
}

// Convenience methods
void Logger::debug(const QString& message, const QString& category)
{
#if _DEBUG
    log(LogLevel::Debug, message, category);
#endif
}

void Logger::info(const QString& message, const QString& category)
{
    log(LogLevel::Info, message, category);
}

void Logger::warning(const QString& message, const QString& category)
{
    log(LogLevel::Warning, message, category);
}

void Logger::error(const QString& message, const QString& category)
{
    log(LogLevel::Error, message, category);
}

void Logger::critical(const QString& message, const QString& category)
{
    log(LogLevel::Critical, message, category);
}

void Logger::setLoggingEnabled(bool enabled)
{
    QMutexLocker locker(&m_mutex);
    m_enabled = enabled;
}