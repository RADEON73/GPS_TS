#include "NmeaGGA.h"
#include "NmeaIParser.h"
#include "Settings.h"
#include <Logger.h>
#include <qstringlist.h>

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