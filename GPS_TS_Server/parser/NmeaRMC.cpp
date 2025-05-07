#include "Logger.h"
#include "NmeaIParser.h"
#include "NmeaRMC.h"
#include "Settings.h"
#include <qstringlist.h>

NmeaIData nmeaRMC::parse(const QStringList & str)
{
	RMC_Data data_;
	if (str.size() == 13) {
		data_.Message_ID = str[0];
		data_.UTC_Time = str[1];
		data_.Status = str[2];
		data_.Latitude = str[3];
		data_.NS_Indicator = str[4];
		data_.Longitude = str[5];
		data_.EW_Indicator = str[6];
		data_.Speed_over_ground = str[7];
		data_.Course_over_ground = str[8];
		data_.Date = str[9];
		data_.Magnetic_variation = str[10];
		data_.Variation_sense = str[11];
		data_.Mode = str[12].left(1);
		data_.Checksum = str[12].right(2);
	}
	if (Settings::instance().logging().logRMC)
		Logger::instance().info(str.join(","));
	return data_;
}

void nmeaRMC::process()
{
	//Не реализовано
}