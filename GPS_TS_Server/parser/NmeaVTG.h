#pragma once
#include "NmeaIParser.h"
#include <memory>

class nmeaVTG : public nmeaIParser //VTG - Course over ground and ground speed
{
public:
	nmeaVTG() = default;
	~nmeaVTG() final = default;

private:
	struct Data
	{
		QString Message_ID; //$GPVTG GGA protocol header
		QString Course_over_ground_1; //34.92 degrees Measured heading
		QString Reference_1; //T True
		QString Course_over_ground_2; //degrees Measured heading
		QString Reference_2; //M Magnetic
		QString Speed_over_ground_1; //0.01 knots Measured speed
		QString Units_1; //N Knots
		QString Speed_over_ground_2;
		QString Units_2; //K Kilometer per hour
		QString Mode;	//A = autonomous,
					//D = DGPS,
					//E = DR,
					//N = Data not valid,
					//R = Coarse Position,
					//S = Simulator
		QString Checksum; //*4D
		//<CR> <LF>
	};

	std::unique_ptr<Data> data = std::make_unique<Data>();

public:
	void parse(const QStringList& str) override;
	void process() override;
};