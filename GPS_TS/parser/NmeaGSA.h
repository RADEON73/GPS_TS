#pragma once
#include "NmeaIParser.h"
#include <memory>

class nmeaGSA : public nmeaIParser //GSA---GNSS DOP and Active Satellites
{
public:
	nmeaGSA() = default;
	~nmeaGSA() final = default;

private:
	struct Data
	{
		QString Message_ID; //$GPGSA protocol header
		QString Mode1; //M Manual- forced to operate in 2D or 3D mode
					//A Automatic - allowed to automatically switch 2D / 3D
		QString Mode2; //1 Fix not available
					//2 2D
					//3 3D
		QString ID_of_satellite_used_1; //Sv on Channel 1
		QString ID_of_satellite_used_2; //Sv on Channel 1
		QString ID_of_satellite_used_3; //Sv on Channel 1
		QString ID_of_satellite_used_4; //Sv on Channel 1
		QString ID_of_satellite_used_5; //Sv on Channel 1
		QString ID_of_satellite_used_6; //Sv on Channel 1
		QString ID_of_satellite_used_7; //Sv on Channel 1
		QString ID_of_satellite_used_8; //Sv on Channel 1
		QString ID_of_satellite_used_9; //Sv on Channel 1
		QString ID_of_satellite_used_10; //Sv on Channel 1
		//...возможно их больше, надо проверять
		QString PDOP; //1.44  Position Dilution of Precision
		QString HDOP; //0.68 Horizontal Dilution of Precision
		QString VDOP; //1.27 Vertical Dilution of Precision
		QString Checksum; //*66
		//<CR> <LF>
	};

	std::unique_ptr<Data> data = std::make_unique<Data>();

public:
	void parse(const QStringList& str) override;
	void process() override;
};
