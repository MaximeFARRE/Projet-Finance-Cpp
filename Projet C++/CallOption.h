#pragma once
#include "EuropeanVanillaOption.h"

class CallOption : public EuropeanVanillaOption {
public:
    CallOption(double expiry, double strike);

    virtual double payoff(double spot) const override;
    virtual optionType GetOptionType() const override;
};
