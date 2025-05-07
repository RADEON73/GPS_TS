#pragma once
#include "NmeaIParser.h"
#include <qstringlist.h>

class nmeaVTG : public nmeaIParser //VTG - Course over ground and ground speed
{
public:
	nmeaVTG() = default;
	~nmeaVTG() final = default;

	NmeaIData parse(const QStringList& str) override;
	void process() override;
};