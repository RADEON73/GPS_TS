#include "Logger.h"
#include "NmeaIParser.h"
#include "NmeaVTG.h"
#include "Settings.h"
#include <qstringlist.h>

NmeaIData nmeaVTG::parse(const QStringList & str)
{
	VTG_Data data_;

	if (Settings::instance().logging().logVTG)
		Logger::instance().info(str.join(","));
	return data_;
}

void nmeaVTG::process()
{
	//Не реализовано
}