#include "NmeaGGA.h"
#include "../core/Settings.h"
#include "../core/Logger.h"

NmeaIData nmeaGGA::parse(const QStringList& str)
{
	GGA_Data data_;
	if (Settings::instance().logging().logGGA)
		Logger::instance().info(str.join(","));
	return data_;
}

void nmeaGGA::process() 
{
	//Не реализовано
}