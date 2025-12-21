#pragma once
#include "EuropeanVanillaOption.h"

// CallOption is a child class of EuropeanVanillaOption
class CallOption : public EuropeanVanillaOption {
public:
    // Constructor
    CallOption(double expiry, double strike);

    // Indicates that this is a call option
    virtual optionType GetOptionType() const override;

    //  payoff
    virtual double payoff(double z) const override;
};
//