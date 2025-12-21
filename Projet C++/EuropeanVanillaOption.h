#pragma once
#include "Option.h"

// EuropeanVanillaOption is a child class of Option
class EuropeanVanillaOption : public Option {
public:
    // option type 
    enum optionType { call, put };

private:
    double _strike; 
    friend class BlackScholesPricer;


public:
    // Constructor 
    EuropeanVanillaOption(double expiry, double strike);

    // Getter 
    double getStrike() const { 
        return _strike; 
    }
    
    // Each option must specify whether it is a call or a put
    virtual optionType GetOptionType() const = 0;
};
