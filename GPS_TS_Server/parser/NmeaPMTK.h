#pragma once
#include "NmeaIParser.h"

class Nmea_PMTK : public nmeaIParser
{
public:
	Nmea_PMTK() = default;
	~Nmea_PMTK() final = default;

	NmeaIData parse(const QStringList& str) override;
	void process() override;
};