#pragma once
#include "EuropeanDigitalOption.h"

// Digital Call : payo = 1 si S >= K, 0 sinon
class EuropeanDigitalCallOption : public EuropeanDigitalOption {
public:
    EuropeanDigitalCallOption(double expiry, double strike);

    virtual double payoff(double spot) const override;
    virtual optionType GetOptionType() const override;
};
//