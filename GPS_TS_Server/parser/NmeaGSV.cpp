#include "Logger.h"
#include "NmeaGSV.h"
#include "NmeaIParser.h"
#include "Settings.h"
#include <qstringlist.h>

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