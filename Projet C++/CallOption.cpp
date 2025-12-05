#include "CallOption.h"

CallOption::CallOption(double expiry, double strike)
    : EuropeanVanillaOption(expiry, strike) {
}

double CallOption::payoff(double spot) const {
    // payoff max(spot - K, 0)
    double K = _strike;
    if (spot > K) {
        return spot - K;
    }
    return 0.0;
}

EuropeanVanillaOption::optionType CallOption::GetOptionType() const {
    return EuropeanVanillaOption::call;
}
