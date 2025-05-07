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

Logger& Logger::instance()
{
    static Logger instance;
    return instance;
}

Logger::Logger(QObject* parent) : QObject(parent), m_consoleOutput(true), m_enabled(true)
{}

Logger::~Logger()
{
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
}

void Logger::init(const QString& logFilePath, bool consoleOutput)
{
    QMutexLocker locker(&m_mutex);

    m_consoleOutput = consoleOutput;

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

    if (m_consoleOutput) {
        qDebug().noquote() << formatted;
    }

    if (m_logFile.isOpen()) {
        m_fileStream << formatted << Qt::endl;
        m_fileStream.flush();
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

    if (!category.isEmpty()) {
        formatted += QString(" [%1]").arg(category);
    }

    formatted += QString(": %1").arg(message);

    return formatted;
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