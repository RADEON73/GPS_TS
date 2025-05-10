#pragma once
#include "NmeaIParser.h"
#include <memory>

class nmeaGLL : public nmeaIParser //GLL--- Geographic Position – Latitude/Longitude
{
public:
	nmeaGLL() = default;

	~nmeaGLL() final = default;

	NmeaIData parse(const QStringList& str) override;
	void process() override;
};
