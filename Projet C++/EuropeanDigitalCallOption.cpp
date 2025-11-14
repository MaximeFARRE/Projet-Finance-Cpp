#include "EuropeanDigitalCallOption.h"

EuropeanDigitalCallOption::EuropeanDigitalCallOption(double expiry, double strike)
    : EuropeanDigitalOption(expiry, strike)
{
}

EuropeanDigitalOption::OptionType EuropeanDigitalCallOption::GetOptionType() const {
    return OptionType::call;
}

double EuropeanDigitalCallOption::payoff(double z) const {
    // Digital Call : paye 1 si z >= K, 0 sinon
    return (z >= getStrike()) ? 1.0 : 0.0;
}
