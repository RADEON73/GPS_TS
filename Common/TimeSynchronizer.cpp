#include "Logger.h"
#include "TimeSynchronizer.h"
#include <qsystemdetection.h>
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
	return m_timeVariable.toBinary();
}

void TimeSynchronizer::timeFromBinary(const QByteArray& timeData)
{
	m_timeVariable.fromBinary(timeData);
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
	if (m_timeVariable.isValid) {
		if (!SetLocalTime(&newTime))
			Logger::instance().warning("Ошибка при попытке установки системного времени");
		else
			Logger::instance().info("Время было успешно установлено " + systemTimeToString(newTime));
	}
	else
		Logger::instance().warning("Данные времени не валидны. Невозможно установить системное время.");

#endif
}

void TimeSynchronizer::setTime(const DateTimePacket& timeData)
{
	m_timeVariable = timeData;
}

#ifdef Q_OS_WIN
SYSTEMTIME TimeSynchronizer::time() const
{
	SYSTEMTIME st{};
	QDateTime localDt = m_timeVariable.dateTime.toLocalTime(); //С учетом часового пояса
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