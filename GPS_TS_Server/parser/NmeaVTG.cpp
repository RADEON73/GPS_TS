#include "NmeaVTG.h"
#include "../core/Settings.h"
#include "../core/Logger.h"

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