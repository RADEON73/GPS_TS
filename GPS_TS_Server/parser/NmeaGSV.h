#pragma once
#include "NmeaIParser.h"
#include <qstringlist.h>

class nmeaGSV : public nmeaIParser //GSV---GNSS Satellites in View
{
public:
	nmeaGSV() = default;
	~nmeaGSV() final = default;

	NmeaIData parse(const QStringList& str) override;
	void process() override;
};
