#pragma once
#include "NmeaIParser.h"
#include <memory>
#include <QDateTime>

class nmeaRMC : public nmeaIParser //RMC---Recommended Minimum Specific GNSS Data
{
public:
	nmeaRMC() = default;
	~nmeaRMC() final = default;

private:
	struct Data
	{
		QString Message_ID; //$GPRMC GGA protocol header
		QString UTC_Time; //183015.000 hhmmss.sss
		QString Status; //A=data valid or V=data not valid
		QString Latitude; //2503.7123 ddmm.mmmm
		QString NS_Indicator; //N=north or S=south
		QString Longitude; //12138.7446 dddmm.mmmm
		QString EW_Indicator; //E=east or W=west
		QString Speed_over_ground; //0.01 knots True
		QString Course_over_ground; //34.92 degrees
		QString Date; //270812 ddmmyy
		QString Magnetic_variation; //degrees
		QString Variation_sense; //E = east or W = west(Not shown)
		QString Mode;	//A = autonomous,
					//D = DGPS,
					//E = DR,
					//N = Data not valid,
					//R = Coarse Position,
					//S = Simulator
		QString Checksum; //*43
		//<CR> <LF>
	};

	std::unique_ptr<Data> data = std::make_unique<Data>();

public:
	void parse(const QStringList& str) override;
	void process() override;

private:
	QDateTime parseGPSTime() const;
};