#include "NmeaGSV.h"
#include "../core/Settings.h"
#include "../core/Logger.h"

NmeaIData nmeaGSV::parse(const QStringList & str)
{
	GSV_Data data_;
	if (Settings::instance().logging().logGSV)
		Logger::instance().info(str.join(","));
	return data_;
}

void nmeaGSV::process()
{
	//Не реализовано
}