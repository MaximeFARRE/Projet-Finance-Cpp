#include "AmericanCallOption.h"

AmericanCallOption::AmericanCallOption(double expiry, double strike)
    : AmericanOption(expiry, strike) {
}

double AmericanCallOption::payoff(double spot) const {
    if (spot > _strike) {
        return spot - _strike;
    }
    return 0.0;
}
// 