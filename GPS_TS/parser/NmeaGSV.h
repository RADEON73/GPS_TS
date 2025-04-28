#pragma once
#include "NmeaIParser.h"
#include <memory>

class nmeaGSV : public nmeaIParser //GSV---GNSS Satellites in View
{
public:
	nmeaGSV() = default;
	~nmeaGSV() final = default;

private:
	struct Data
	{
		QString Message_ID; //$GPGSV protocol header
		QString Total_number_of_messages; //Range 1 to 6
		QString Message_number; //Range 1 to 6
		QString Satellites_in_view; //11
		QString Satellite_ID_1; //Channel 1 (Range 01 to 196)
		QString Elevation_1; //degrees Channel 1 (Range 00 to 90)
		QString Azimuth_1; //degrees Channel 1 (Range 000 to 359)
		QString SNR_1; //dB - Hz Channel 1 (Range 00 to 99, null when not tracking)
		QString Satellite_ID_4; //Channel 4 (Range 01 to 196)
		QString Elevation_4; //degrees Channel 4 (Range 00 to 90)
		QString Azimuth_4; //degrees Channel 4 (Range 000 to 359)
		QString SNR_4; //dB - Hz Channel 4 (Range 00 to 99, null when not tracking)
		QString Checksum; //*4D
		//<CR> <LF>
	};

	std::unique_ptr<Data> data = std::make_unique<Data>();

public:
	void parse(const QStringList& str) override;
	void process() override;
};
