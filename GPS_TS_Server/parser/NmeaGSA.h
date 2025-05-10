#pragma once
#include "NmeaIParser.h"
#include <memory>

class nmeaGSA : public nmeaIParser //GSA---GNSS DOP and Active Satellites
{
public:
	nmeaGSA() = default;
	~nmeaGSA() final = default;

	NmeaIData parse(const QStringList& str) override;
	void process() override;
};
