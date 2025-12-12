#include "PutOption.h"

PutOption::PutOption(double expiry, double strike)
    : EuropeanVanillaOption(expiry, strike) {
}

double PutOption::payoff(double spot) const {
    // payoff max(K - spot, 0)
    double K = _strike;
    if (spot < K) {
        return K - spot;
    }
    return 0.0;
}
//
EuropeanVanillaOption::optionType PutOption::GetOptionType() const {
    return EuropeanVanillaOption::put;
}
