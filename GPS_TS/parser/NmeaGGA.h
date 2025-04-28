#pragma once
#include "NmeaIParser.h"
#include <memory>
#include <memory>

class nmeaGGA : public nmeaIParser //GGA--- Global Positioning System Fixed Data
{
public:
	nmeaGGA() = default;
	~nmeaGGA() final = default;

private:
	struct Data
	{
		QString Message_ID; //$GPGGA protocol header
		QString UTC_Time; //183015.000 hhmmss.sss
		QString Latitude; //2503.7123 ddmm.mmmm
		QString NS_Indicator; //N=north or S=south
		QString Longitude; //12138.7446 dddmm.mmmm
		QString EW_Indicator; //E=east or W=west
		QString Position_Fix_Indicator; //0 Fix not available or invalid ,
											 //1 GPS SPS Mode, fix valid,
											 //2 Differential GPS, SPS Mode, fix valid,
											 //3 - 5 Not supported,
											 //6 Dead Reckoning Mode, fix valid
		QString Satellites_Used; //Range 0 to 33
		QString HDOP; //0.68 Horizontal Dilution of Precision
		QString MSL_Altitude; //123.2 mters
		QString Units_1; //M mters
		QString Geoid_Separation; //15.3 mters
		QString Units_2; //M mters
		QString Age_of_Diff_Corr; //0000 second Null fields when DGPS is not used
		QString Diff_Ref_Station_ID; //0000
		QString Checksum; //*66
		//<CR> <LF>
	};

	std::unique_ptr<Data> data = std::make_unique<Data>();

public:
	void parse(const QStringList& str) final;
	void process() final;
};
