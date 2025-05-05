#include "TimeSynchronizer.h"
#include <QDataStream>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
#include "../../GPS_TS_Client/core/Logger.h"

TimeSynchronizer::TimeSynchronizer(QObject* parent) : QObject(parent)
{
}

QByteArray TimeSynchronizer::currentTimeToBinary() const
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << m_timeVariable.toMSecsSinceEpoch(); // UNIX timestamp в миллисекундах
	return data;
}

void TimeSynchronizer::setTimeFromBinary(const QByteArray& timeData)
{
	QDataStream stream(timeData);
	qint64 msecs;
	stream >> msecs;
	m_timeVariable = QDateTime::fromMSecsSinceEpoch(msecs, Qt::UTC);
}

void TimeSynchronizer::synchronizeTime()
{
#ifdef Q_OS_WIN
		if (auto sysTime = time(); !SetLocalTime(&sysTime))
			Logger::instance().warning("Ошибка при попытке установки системного времени");
		else
			Logger::instance().info("Время было успешно установлено");
		return ;
#endif
        emit timeSynchronized(m_timeVariable);
}

void TimeSynchronizer::setTime(const QString& UTSDate, const QString& UTSTime)
{
	// Формат даты: DDMMYY
	// Формат времени: HHMMSS.SSS
	QDate date( // "270812"
		2000 + UTSDate.mid(4, 2).toInt(), // Год
		UTSDate.mid(2, 2).toInt(), // Месяц
		UTSDate.left(2).toInt() // День
	);
	QTime time( // "183015.000"
		UTSTime.left(2).toInt(), // Часы
		UTSTime.mid(2, 2).toInt(), // Минуты
		UTSTime.mid(4, 2).toInt(), // Секунды
		UTSTime.mid(7, 3).toInt() // Миллисекунды
	);
	m_timeVariable = QDateTime(date, time, Qt::UTC);
}

#ifdef Q_OS_WIN
SYSTEMTIME TimeSynchronizer::time() const
{
	SYSTEMTIME st;
	QDateTime localDt = m_timeVariable.toLocalTime(); //С учетом часового пояса
	//QDateTime localDt = dt; //Без учета часового пояса
	st.wYear = static_cast<WORD>(localDt.date().year());
	st.wMonth = static_cast<WORD>(localDt.date().month());
	st.wDay = static_cast<WORD>(localDt.date().day());
	st.wHour = static_cast<WORD>(localDt.time().hour());
	st.wMinute = static_cast<WORD>(localDt.time().minute());
	st.wSecond = static_cast<WORD>(localDt.time().second());
	st.wMilliseconds = static_cast<WORD>(localDt.time().msec());
	return st;
}
#endif