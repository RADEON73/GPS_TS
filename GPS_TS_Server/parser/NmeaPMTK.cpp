#include "NmeaPMTK.h"
#include "../core/Settings.h"
#include "../core/Logger.h"

void Nmea_PMTK::parse(const QStringList & str)
{
	if (Settings::instance().logging().logPMTK)
		Logger::instance().info(str.join(","));
}

void Nmea_PMTK::process()
{
	//Не реализовано
}