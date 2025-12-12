#pragma once
#include "EuropeanDigitalOption.h"

class EuropeanDigitalCallOption : public EuropeanDigitalOption {
public:
    EuropeanDigitalCallOption(double expiry, double strike);

    virtual double payoff(double spot) const override;
    virtual optionType GetOptionType() const override;
};
//