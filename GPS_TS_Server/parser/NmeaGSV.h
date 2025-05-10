#pragma once
#include "NmeaIParser.h"
#include <memory>

class nmeaGSV : public nmeaIParser //GSV---GNSS Satellites in View
{
public:
	nmeaGSV() = default;
	~nmeaGSV() final = default;

	NmeaIData parse(const QStringList& str) override;
	void process() override;
};
