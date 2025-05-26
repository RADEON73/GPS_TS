#include "Logger.h"
#include "TimeSynchronizer.h"
#include <qsystemdetection.h>
#include <qdatastream.h>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

inline QString systemTimeToString(const SYSTEMTIME& st)
{
	QDate date(st.wYear, st.wMonth, st.wDay);
	QTime time(st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	QDateTime dt(date, time);

	return dt.toString("yyyy-MM-dd hh:mm:ss.zzz");
}

inline bool needSync(const SYSTEMTIME& newTime)
{
	SYSTEMTIME current;
	GetLocalTime(&current);

	return newTime.wYear != current.wYear
		|| newTime.wMonth != current.wMonth
		|| newTime.wDay != current.wDay
		|| newTime.wHour != current.wHour
		|| newTime.wMinute != current.wMinute
		|| newTime.wSecond != current.wSecond
		|| abs(newTime.wMilliseconds - current.wMilliseconds) >= 1;
}


TimeSynchronizer::TimeSynchronizer(QObject* parent) : QObject(parent)
{
}

QByteArray TimeSynchronizer::timeToBinary() const
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << m_timeVariable.toMSecsSinceEpoch(); // UNIX timestamp в миллисекундах
	return data;
}

void TimeSynchronizer::timeFromBinary(const QByteArray& timeData)
{
	QDataStream stream(timeData);
	qint64 msecs;
	stream >> msecs;
	m_timeVariable = QDateTime::fromMSecsSinceEpoch(msecs, Qt::UTC);
}

void TimeSynchronizer::synchronizeTime() const
{
#ifdef Q_OS_WIN
	const SYSTEMTIME newTime = time();
#ifndef _DEBUG //Оптимизация релизной сборки
	SYSTEMTIME current;
	GetLocalTime(&current);
	const bool skipSync =
		(newTime.wYear == current.wYear
			&& newTime.wMonth == current.wMonth
			&& newTime.wDay == current.wDay
			&& newTime.wHour == current.wHour
			&& newTime.wMinute == current.wMinute)
		&& (abs(newTime.wSecond - current.wSecond) < 1 && abs(newTime.wMilliseconds - current.wMilliseconds) < 100);
	if (skipSync) {
		Logger::instance().info("Синхронизация не требуется ввиду малого расхождения с системным временем");
		return;
	}
#endif // _DEBUG
	if (!SetLocalTime(&newTime))
		Logger::instance().warning("Ошибка при попытке установки системного времени");
	else
		Logger::instance().info("Время было успешно установлено " + systemTimeToString(newTime));
#endif
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