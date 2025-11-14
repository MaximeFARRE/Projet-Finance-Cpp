#include "EuropeanDigitalPutOption.h"

EuropeanDigitalPutOption::EuropeanDigitalPutOption(double expiry, double strike)
    : EuropeanDigitalOption(expiry, strike)
{
}

EuropeanDigitalOption::OptionType EuropeanDigitalPutOption::GetOptionType() const {
    return OptionType::put;
}

double EuropeanDigitalPutOption::payoff(double z) const {
    // Digital Put : paye 1 si z <= K, 0 sinon
    return (z <= getStrike()) ? 1.0 : 0.0;
}
