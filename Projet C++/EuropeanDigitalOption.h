#pragma once
#include "Option.h"

// EuropeanDigitalOption is a child class of Option
class EuropeanDigitalOption : public Option {
protected:
    double _strike;

public:
    // Constructor
    EuropeanDigitalOption(double expiry, double strike);
    //Destructor
    virtual ~EuropeanDigitalOption() {}

    // Getter
    double getStrike() const;

    enum optionType { call, put };
    virtual optionType GetOptionType() const = 0;

    // Allow the pricer to access the strike
    friend class BlackScholesPricer;
};


