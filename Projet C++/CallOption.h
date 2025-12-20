#pragma once

#include "EuropeanVanillaOption.h"

// European call option
class CallOption : public EuropeanVanillaOption {
public:
    // Constructor: passes expiry and strike to the base class
    CallOption(double expiry, double strike);

    // Indicates that this is a call option
    virtual optionType GetOptionType() const override;

    // Call payoff : max(S - K, 0)
    virtual double payoff(double z) const override;
};
