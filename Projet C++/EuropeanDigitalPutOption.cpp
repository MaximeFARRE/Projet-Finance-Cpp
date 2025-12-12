#include "EuropeanDigitalPutOption.h"

EuropeanDigitalPutOption::EuropeanDigitalPutOption(double expiry, double strike)
    : EuropeanDigitalOption(expiry, strike) {
}

double EuropeanDigitalPutOption::payoff(double spot) const {
    // payoff is 1 if spot <= K, else 0
    double K = _strike;
    if (spot <= K) {
        return 1.0;
    }
    return 0.0;
}

EuropeanVanillaOption::optionType EuropeanDigitalPutOption::GetOptionType() const {
    return EuropeanVanillaOption::put;
}
//