#pragma once
#include "NmeaIParser.h"
#include <memory>

class nmeaGLL : public nmeaIParser //GLL--- Geographic Position – Latitude/Longitude
{
public:
	nmeaGLL() = default;
	~nmeaGLL() final = default;

private:
	struct Data
	{
		QString Message_ID; //$GPGGL GGA protocol header
		QString Latitude; //2503.7123 ddmm.mmmm
		QString NS_Indicator; //N=north or S=south
		QString Longitude; //12138.7446 dddmm.mmmm
		QString EW_Indicator; //E=east or W=west
		QString UTC_Time; //183015.000 hhmmss.sss
		QString Status; //A=data valid or V=data not valid
		QString Mode; //A=autonomous, D=DGPS, E=DR, N=Data not valid, R = Coarse Position, S = Simulator
		QString Checksum; //*59
		//<CR> <LF>
	};

	std::unique_ptr<Data> data = std::make_unique<Data>();

public:
	void parse(const QStringList& str) override;
	void process() override;
};
