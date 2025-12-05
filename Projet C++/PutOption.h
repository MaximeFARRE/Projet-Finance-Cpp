#pragma once
#include "EuropeanVanillaOption.h"

class PutOption : public EuropeanVanillaOption {
public:
    PutOption(double expiry, double strike);

    virtual double payoff(double spot) const override;
    virtual optionType GetOptionType() const override;
};
