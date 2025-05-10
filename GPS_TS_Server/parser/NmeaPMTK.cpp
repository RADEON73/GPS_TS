#include "NmeaPMTK.h"
#include "../core/Settings.h"
#include "../core/Logger.h"

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