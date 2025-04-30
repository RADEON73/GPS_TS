#include "NmeaGGA.h"
#include "../core/Settings.h"
#include "../core/Logger.h"

void nmeaGGA::parse(const QStringList& str)
{
	if (Settings::instance().logging().logGGA)
		Logger::instance().info(str.join(","));
}

void nmeaGGA::process()
{
	//Не реализовано
}