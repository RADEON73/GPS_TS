#include "Logger.h"
#include "NmeaGLL.h"
#include "NmeaIParser.h"
#include "Settings.h"
#include <qstringlist.h>

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