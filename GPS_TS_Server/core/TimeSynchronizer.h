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

    // Установка интервала синхронизации в секундах
    void setSyncInterval(int seconds);

    // Запуск таймера синхронизации
    void startSync();

    // Остановка таймера синхронизации
    void stopSync();

    // Проверка активности таймера
    bool isActive() const;

    // Новые методы для бинарной сериализации
    QByteArray currentTimeToBinary() const;
    void setTimeFromBinary(const QByteArray& timeData);

    QDateTime currentTime() const { return m_timeVariable; }

signals:
    void syncStarted();
    void syncStopped();
    void timeSynchronized(const QDateTime& newTime);

private slots:
    void synchronizeTime();

public slots:
    void setTime(const QString& UTSDate, const QString& UTSTime);
#ifdef Q_OS_WIN
    SYSTEMTIME time() const;
#endif

private:
    QTimer m_syncTimer{ this };
    int m_interval = 60000; // Интервал по умолчанию: 60 секунд (в миллисекундах)
    QDateTime m_timeVariable; //Переменная - хранилище времени
};