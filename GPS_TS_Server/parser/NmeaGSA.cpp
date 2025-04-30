#include "NmeaGSA.h"
#include "../core/Settings.h"
#include "../core/Logger.h"

void nmeaGSA::parse(const QStringList & str)
{
	if (Settings::instance().logging().logGSA)
		Logger::instance().info(str.join(","));
}

void nmeaGSA::process()
{
	//Не реализовано
}