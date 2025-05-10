#pragma once
#include "NmeaIParser.h"
#include <memory>
#include <QDateTime>

class nmeaRMC : public nmeaIParser //RMC---Recommended Minimum Specific GNSS Data
{
public:
	nmeaRMC() = default;
	~nmeaRMC() final = default;

	NmeaIData parse(const QStringList& str) override;
	void process() override;
};