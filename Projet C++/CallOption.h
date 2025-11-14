#pragma once

#include "EuropeanVanillaOption.h"

// Option européenne de type Call
class CallOption : public EuropeanVanillaOption {
public:
    // Constructeur : passe expiry et strike à la classe mère
    CallOption(double expiry, double strike);

    // Indique que c'est un call
    virtual optionType GetOptionType() const override;

    // Payoff du call : max(S - K, 0)
    virtual double payoff(double z) const override;
};
