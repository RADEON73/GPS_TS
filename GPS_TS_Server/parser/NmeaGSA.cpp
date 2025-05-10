#include "NmeaGSA.h"
#include "../core/Settings.h"
#include "../core/Logger.h"

NmeaIData nmeaGSA::parse(const QStringList & str)
{
	GSA_Data data_;
	if (Settings::instance().logging().logGSA)
		Logger::instance().info(str.join(","));
	return data_;
}

void nmeaGSA::process()
{
	//Не реализовано
}