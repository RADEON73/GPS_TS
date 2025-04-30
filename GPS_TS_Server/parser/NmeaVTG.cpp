#include "NmeaVTG.h"
#include "../core/Settings.h"
#include "../core/Logger.h"

void nmeaVTG::parse(const QStringList & str)
{
	if (Settings::instance().logging().logVTG)
		Logger::instance().info(str.join(","));
}

void nmeaVTG::process()
{
	//Не реализовано
}