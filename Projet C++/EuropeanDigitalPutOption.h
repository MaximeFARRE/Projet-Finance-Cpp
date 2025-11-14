#pragma once

#include "EuropeanDigitalOption.h"

// Digital Put : payo� = 1 si S <= K, 0 sinon
class EuropeanDigitalPutOption : public EuropeanDigitalOption {
public:
    EuropeanDigitalPutOption(double expiry, double strike);

    virtual OptionType GetOptionType() const override;

    // h(z) = 1_{z <= K}
    virtual double payoff(double z) const override;
};
