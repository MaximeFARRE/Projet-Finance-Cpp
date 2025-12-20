#pragma once

#include "Option.h"

// Abstract class for vanilla European options (call/put)
class EuropeanVanillaOption : public Option {
public:
    // option type : call or put
    enum optionType { call, put };

private:
    double _strike;  // Strike price K
    friend class BlackScholesPricer;


public:
    // Constructor : initialize expiry (T) and strike (K)
    EuropeanVanillaOption(double expiry, double strike);

    // Getter for the strike price
    double getStrike() const { 
        return _strike; 
    }
    
    // Each option must specify whether it is a call or a put
    virtual optionType GetOptionType() const = 0;
};
