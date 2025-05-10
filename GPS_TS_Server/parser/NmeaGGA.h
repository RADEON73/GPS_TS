#pragma once
#include "NmeaIParser.h"
#include <memory>
#include <memory>

class nmeaGGA : public nmeaIParser //GGA--- Global Positioning System Fixed Data
{
public:
	nmeaGGA() = default;
	~nmeaGGA() final = default;

	NmeaIData parse(const QStringList& str) final;
	void process() final;
};
