#include "Logger.h"
#include "NmeaGSA.h"
#include "NmeaIParser.h"
#include "Settings.h"
#include <qstringlist.h>

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