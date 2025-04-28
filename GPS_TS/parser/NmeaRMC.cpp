#include "NmeaRMC.h"
#ifdef Q_OS_WIN
#include <windows.h>
#elif defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
#include <sys/time.h>
#endif

void nmeaRMC::parse(const QStringList & vStr)
{
	if (vStr.size() != 13) 
		return;
	data->Message_ID = vStr[0];
	data->UTC_Time = vStr[1];
	data->Status = vStr[2];
	data->Latitude = vStr[3];
	data->NS_Indicator = vStr[4];
	data->Longitude = vStr[5];
	data->EW_Indicator = vStr[6];
	data->Speed_over_ground = vStr[7];
	data->Course_over_ground = vStr[8];
	data->Date = vStr[9];
	data->Magnetic_variation = vStr[10];
	data->Variation_sense = vStr[11];
	data->Mode = vStr[12].left(1);
	data->Checksum = vStr[12].right(2);
}

bool setSystemTimeNative(const QDateTime& dt)
{
	if (!dt.isValid())
		return false;
#ifdef Q_OS_WIN
	SYSTEMTIME st;
	QDateTime localDt = dt.toLocalTime(); //С учетом часового пояса
	//QDateTime localDt = dt; //Без учета часового пояса
	st.wYear = static_cast<WORD>(localDt.date().year());
	st.wMonth = static_cast<WORD>(localDt.date().month());
	st.wDay = static_cast<WORD>(localDt.date().day());
	st.wHour = static_cast<WORD>(localDt.time().hour());
	st.wMinute = static_cast<WORD>(localDt.time().minute());
	st.wSecond = static_cast<WORD>(localDt.time().second());
	st.wMilliseconds = static_cast<WORD>(localDt.time().msec());
	return SetLocalTime(&st);
#else //TODO: На линуксе не проверялось, возможно не работает
	struct timeval tv;
	QDateTime utcDt = dt.toUTC();
	tv.tv_sec = utcDt.toSecsSinceEpoch();
	tv.tv_usec = utcDt.time().msec() * 1000;
	if (settimeofday(&tv, nullptr) == -1) {
		perror("settimeofday failed");
		return false;
	}
	// Обновляем аппаратные часы (RTC) для Linux
#ifdef Q_OS_LINUX
	system("hwclock --systohc");
#endif
	return true;
#endif
}

void nmeaRMC::process()
{
	QDateTime gpsTime = parseGPSTime(); // Парсим дату/время из NMEA

	if (gpsTime.isValid()) {
		setSystemTimeNative(gpsTime); //Установка системного времени (требует прав администратора)
		// Отправка клиентам через TCP (пример)
		//QByteArray timeData = serializeDateTime(gpsTime);
		//emit timeDataReady(timeData);
	}

	//SYSTEMTIME SystemTime;
	//GetSystemTime(&SystemTime);
	//SystemTime = ParseGPSDateTime();
	//SetSystemTime(&SystemTime);
	//m_ui->setGpsMessage(m_ui->parseMessage(&SystemTime));
	//m_ui->timeSyncProc(m_ui->getGpsMessage(), &SystemTime);
	//m_ui->setSatStatus(data->Status);
	//auto datagram = QByteArray(static_cast<char*>((void*)&SystemTime), sizeof(SystemTime));
	//m_ui->getTcpServer()->SendToClient(datagram);
}

QDateTime nmeaRMC::parseGPSTime() const
{
	// Формат даты: DDMMYY
	// Формат времени: HHMMSS.SSS
	QString dateStr = data->Date; // "270812"
	QString timeStr = data->UTC_Time; // "183015.000"
	QDate date(
		2000 + dateStr.mid(4, 2).toInt(), // Год
		dateStr.mid(2, 2).toInt(), // Месяц
		dateStr.left(2).toInt() // День
	);
	QTime time(
		timeStr.left(2).toInt(), // Часы
		timeStr.mid(2, 2).toInt(), // Минуты
		timeStr.mid(4, 2).toInt(), // Секунды
		timeStr.mid(7, 3).toInt() // Миллисекунды
	);
	return QDateTime(date, time, Qt::UTC);

	/* Windows спефицично
	SYSTEMTIME GpsTime;
	GetSystemTime(&GpsTime);
	GpsTime.wDay = data->Date.left(2).toUShort();
	GpsTime.wMonth = data->Date.left(4).right(2).toUShort();
	GpsTime.wYear = 2000 + data->Date.right(2).toUShort();
	GpsTime.wHour = data->UTC_Time.left(2).toUShort();
	GpsTime.wMinute = data->UTC_Time.left(4).right(2).toUShort();
	GpsTime.wSecond = data->UTC_Time.left(6).right(2).toUShort();
	GpsTime.wMilliseconds = data->UTC_Time.right(3).toUShort();
	return GpsTime;
	*/
}