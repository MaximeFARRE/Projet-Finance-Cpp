#include "PutOption.h"
#include <algorithm> // for std::max

// Constructor
PutOption::PutOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike){}

// Getter
EuropeanVanillaOption::optionType PutOption::GetOptionType() const {
    return optionType::put;
}

// payoff
double PutOption::payoff(double z) const {
    return std::max(getStrike() - z, 0.0);
}
