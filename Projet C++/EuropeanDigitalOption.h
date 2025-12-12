#pragma once
#include "EuropeanVanillaOption.h"

// digital base class, similar to EuropeanVanillaOption
class EuropeanDigitalOption : public EuropeanVanillaOption {
public:
    EuropeanDigitalOption(double expiry, double strike);

    virtual ~EuropeanDigitalOption() {}
};
//