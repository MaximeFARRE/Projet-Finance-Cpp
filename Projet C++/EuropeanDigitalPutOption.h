#pragma once
#include "EuropeanDigitalOption.h"

class EuropeanDigitalPutOption : public EuropeanDigitalOption {
public:
    EuropeanDigitalPutOption(double expiry, double strike);

    virtual double payoff(double spot) const override;
    virtual optionType GetOptionType() const override;
};
