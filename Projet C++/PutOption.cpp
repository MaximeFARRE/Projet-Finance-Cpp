#include "PutOption.h"
#include <algorithm> // pour std::max

// Constructeur : transmet expiry et strike à EuropeanVanillaOption
PutOption::PutOption(double expiry, double strike)
    : EuropeanVanillaOption(expiry, strike)
{
}

// Type de l'option : put
EuropeanVanillaOption::optionType PutOption::GetOptionType() const {
    return optionType::put;
}

// Payoff du put : max(K - S, 0)
double PutOption::payoff(double z) const {
    return std::max(getStrike() - z, 0.0);
}
