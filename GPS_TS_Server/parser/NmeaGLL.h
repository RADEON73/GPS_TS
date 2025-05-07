#pragma once
#include "NmeaIParser.h"
#include <qstringlist.h>

class nmeaGLL : public nmeaIParser //GLL--- Geographic Position – Latitude/Longitude
{
public:
	nmeaGLL() = default;

	~nmeaGLL() final = default;

	NmeaIData parse(const QStringList& str) override;
	void process() override;
};
