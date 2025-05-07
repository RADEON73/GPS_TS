#pragma once
#include "NmeaGGA.h"
#include "NmeaGLL.h"
#include "NmeaGSA.h"
#include "NmeaGSV.h"
#include "NmeaIParser.h"
#include "NmeaPMTK.h"
#include "NmeaRMC.h"
#include "NmeaVTG.h"
#include <memory>

class NmeaParserFactory
{
public:
    static std::unique_ptr<nmeaIParser> createParser(const QString& sentence)
    {
        if (sentence.left(5) == "$PMTK")
            return std::make_unique<Nmea_PMTK>();
        const QString type = sentence.mid(3, 3);
        if (type == "RMC") 
            return std::make_unique<nmeaRMC>();
        if (type == "GGA") 
            return std::make_unique<nmeaGGA>();
        if (type == "GLL")
            return std::make_unique<nmeaGLL>();
        if (type == "GSA")
            return std::make_unique<nmeaGSA>();
        if (type == "GSV")
            return std::make_unique<nmeaGSV>();
        if (type == "VTG")
            return std::make_unique<nmeaVTG>();
        return nullptr;
    }
};