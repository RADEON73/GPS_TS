#pragma once
#include <QObject>
#include <QTimer>
#include <QDateTime>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

class TimeSynchronizer : public QObject
{
    Q_OBJECT

public:
    explicit TimeSynchronizer(QObject* parent = nullptr);

    // Новые методы для бинарной сериализации
    QByteArray timeToBinary() const;
    void timeFromBinary(const QByteArray& timeData);

    QDateTime currentTime() const { return m_timeVariable; }

signals:
    void syncStarted();
    void syncStopped();
    void timeSynchronized(const QDateTime& newTime);

public slots:
    void synchronizeTime();
    void setTime(const QString& UTSDate, const QString& UTSTime);

#ifdef Q_OS_WIN
    SYSTEMTIME time() const;
#endif

private:
    int m_interval = 60000; // Интервал по умолчанию: 60 секунд (в миллисекундах)
    QDateTime m_timeVariable; //Переменная - хранилище времени
};