#include "PutOption.h"
#include <algorithm> // for std::max

// Constructor: passes expiry and strike to EuropeanVanillaOption
PutOption::PutOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike)
{}

// Option type : put
EuropeanVanillaOption::optionType PutOption::GetOptionType() const {
    return optionType::put;
}

// Put payoff : max(K - S, 0)
double PutOption::payoff(double z) const {
    return std::max(getStrike() - z, 0.0);
}
