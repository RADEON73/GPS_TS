#pragma once
#include "NmeaIParser.h"
#include <memory>

class Nmea_PMTK : public nmeaIParser
{
public:
	Nmea_PMTK() = default;
	~Nmea_PMTK() final = default;

private:
	struct Data
	{
		QString Message_ID; //$PMTK
		QString DataPack;
		QString Checksum;
		//<CR> <LF>
	};

	std::unique_ptr<Data> data = std::make_unique<Data>();

public:
	void parse(const QStringList& str) override;
	void process() override;
};