#include "NmeaGLL.h"
#include "../core/Settings.h"
#include "../core/Logger.h"

NmeaIData nmeaGLL::parse(const QStringList & str)
{
	GLL_Data data_;
	if (Settings::instance().logging().logGLL)
		Logger::instance().info(str.join(","));
	return data_;
}

void nmeaGLL::process()
{
	//Не реализовано
}