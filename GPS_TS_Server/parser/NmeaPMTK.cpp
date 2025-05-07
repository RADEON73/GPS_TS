#include "Logger.h"
#include "NmeaIParser.h"
#include "NmeaPMTK.h"
#include "Settings.h"
#include <qstringlist.h>

NmeaIData Nmea_PMTK::parse(const QStringList & str)
{
	PMTK_Data data_;
	if (Settings::instance().logging().logPMTK)
		Logger::instance().info(str.join(","));
	return data_;
}

void Nmea_PMTK::process()
{
	//Не реализовано
}