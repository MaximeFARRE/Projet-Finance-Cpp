#pragma once

#include "EuropeanVanillaOption.h"

// European put-type option
class PutOption : public EuropeanVanillaOption {
public:
    // Constructor: passes expiry and strike to the base class
    PutOption(double expiry, double strike);

    // Indicates that this is a put option
    virtual optionType GetOptionType() const override;

    // Put payoff: max(K - S, 0)
    virtual double payoff(double z) const override;
};
