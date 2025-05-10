#pragma once
#include <QStringList>
#include <variant>

struct PMTK_Data
{
	QString Message_ID; //$PMTK
	QString DataPack;
	QString Checksum;
	//<CR> <LF>
};

struct VTG_Data
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

struct GSV_Data
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

struct GSA_Data
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

struct RMC_Data
{
	QString Message_ID; //$GPRMC GGA protocol header
	QString UTC_Time; //183015.000 hhmmss.sss
	QString Status; //A=data_ valid or V=data_ not valid
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

struct GGA_Data
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

struct GLL_Data
{
	QString Message_ID; //$GPGGL GGA protocol header
	QString Latitude; //2503.7123 ddmm.mmmm
	QString NS_Indicator; //N=north or S=south
	QString Longitude; //12138.7446 dddmm.mmmm
	QString EW_Indicator; //E=east or W=west
	QString UTC_Time; //183015.000 hhmmss.sss
	QString Status; //A=data_ valid or V=data_ not valid
	QString Mode; //A=autonomous, D=DGPS, E=DR, N=Data not valid, R = Coarse Position, S = Simulator
	QString Checksum; //*59
	//<CR> <LF>
};

using NmeaIData = std::variant<std::monostate, GLL_Data, GSA_Data, GSV_Data, GGA_Data, RMC_Data, VTG_Data, PMTK_Data>;

class nmeaIParser
{
public:
	virtual ~nmeaIParser() = default;

	virtual NmeaIData parse(const QStringList& str) = 0;
	virtual void process() = 0;
};