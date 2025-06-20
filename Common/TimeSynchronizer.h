#pragma once
#include <qsystemdetection.h>
#include <qbytearray.h>
#include <qdatetime.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qdatastream.h>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

struct DateTimePacket
{
    DateTimePacket() = default;
    DateTimePacket(const QString& date_, const QString& time_, const QString& status_)
    {
        // Формат даты: DDMMYY
        // Формат времени: HHMMSS.SSS

        QDate date( // "270812"
            2000 + date_.mid(4, 2).toInt(), // Год
            date_.mid(2, 2).toInt(), // Месяц
            date_.left(2).toInt() // День
        );
        QTime time( // "183015.000"
            time_.left(2).toInt(), // Часы
            time_.mid(2, 2).toInt(), // Минуты
            time_.mid(4, 2).toInt(), // Секунды
            time_.mid(7, 3).toInt() // Миллисекунды
        );
        dateTime = QDateTime(date, time, Qt::UTC);
		isValid = (status_ == 'A');
    }

    // Методы для бинарной сериализации/десериализации
    QByteArray toBinary() const
    {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << dateTime.toMSecsSinceEpoch() << isValid;
        return data;
    }

    void fromBinary(const QByteArray& data)
    {
        QDataStream stream(data);
        qint64 msecs;
        stream >> msecs >> isValid;
        dateTime = QDateTime::fromMSecsSinceEpoch(msecs, Qt::UTC);
    }

    QDateTime dateTime;
    bool isValid = false;
};

class TimeSynchronizer : public QObject
{
    Q_OBJECT

public:
    explicit TimeSynchronizer(QObject* parent = nullptr);

    // Новые методы для бинарной сериализации
    QByteArray timeToBinary() const;
    void timeFromBinary(const QByteArray& timeData);

    QDateTime currentTime() const { return m_timeVariable.dateTime; }

public slots:
    void synchronizeTime() const;
    void setTime(const DateTimePacket& timeData);

#ifdef Q_OS_WIN
    SYSTEMTIME time() const;
#endif

private:
    int m_interval = 60000; // Интервал по умолчанию: 60 секунд (в миллисекундах)
    DateTimePacket m_timeVariable; //Переменная - хранилище времени
};