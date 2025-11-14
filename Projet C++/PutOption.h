#pragma once

#include "EuropeanVanillaOption.h"

// Option européenne de type Put
class PutOption : public EuropeanVanillaOption {
public:
    // Constructeur : passe expiry et strike à la classe mère
    PutOption(double expiry, double strike);

    // Indique que c'est un put
    virtual optionType GetOptionType() const override;

    // Payoff du put : max(K - S, 0)
    virtual double payoff(double z) const override;
};
