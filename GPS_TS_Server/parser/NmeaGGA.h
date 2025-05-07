#pragma once
#include "NmeaIParser.h"
#include <qstringlist.h>

class nmeaGGA : public nmeaIParser //GGA--- Global Positioning System Fixed Data
{
public:
	nmeaGGA() = default;
	~nmeaGGA() final = default;

	NmeaIData parse(const QStringList& str) final;
	void process() final;
};
