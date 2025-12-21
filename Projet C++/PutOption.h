#pragma once
#include "EuropeanVanillaOption.h"

// PutOption is a child class of EuropeanVanillaOption
class PutOption : public EuropeanVanillaOption {
public:
    // Constructor
    PutOption(double expiry, double strike);

    // getter
    virtual optionType GetOptionType() const override;

    // payoff
    virtual double payoff(double z) const override;
};
