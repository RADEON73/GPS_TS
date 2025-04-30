#include "NmeaGLL.h"
#include "../core/Settings.h"
#include "../core/Logger.h"

void nmeaGLL::parse(const QStringList & str)
{
	if (Settings::instance().logging().logGLL)
		Logger::instance().info(str.join(","));
}

void nmeaGLL::process()
{
	//Не реализовано
}