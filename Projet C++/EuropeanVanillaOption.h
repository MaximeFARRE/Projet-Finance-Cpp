#pragma once
#include "Option.h"
//
class EuropeanVanillaOption : public Option {
public:
    enum optionType { call, put };

protected:
    // strike is protected so children (Call, Put, Digital...) can use it
    double _strike;

public:
    // constructor: set expiry and strike
    EuropeanVanillaOption(double expiry, double strike);

    // friend so pricer can read the strike
    friend class BlackScholesPricer;

    // each option must tell if it is call or put
    virtual optionType GetOptionType() const = 0;
};
