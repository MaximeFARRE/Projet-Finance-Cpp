#include "AmericanPutOption.h"

AmericanPutOption::AmericanPutOption(double expiry, double strike)
    : AmericanOption(expiry, strike) {
}

double AmericanPutOption::payoff(double spot) const {
    if (spot < _strike) {
        return _strike - spot;
    }
    return 0.0;
}
//