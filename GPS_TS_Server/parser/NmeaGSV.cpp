#include "NmeaGSV.h"
#include "../core/Settings.h"
#include "../core/Logger.h"

void nmeaGSV::parse(const QStringList & str)
{
	if (Settings::instance().logging().logGSV)
		Logger::instance().info(str.join(","));
}

void nmeaGSV::process()
{
	//Не реализовано
}