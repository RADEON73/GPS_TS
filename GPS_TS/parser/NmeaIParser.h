#pragma once
#include <QStringList>

class nmeaIParser
{
public:
	virtual ~nmeaIParser() = default;

	virtual void parse(const QStringList& str) = 0;
	virtual void process() = 0;
};