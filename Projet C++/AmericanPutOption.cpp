#include "AmericanPutOption.h"

// Constructor
AmericanPutOption::AmericanPutOption(double expiry, double strike) : AmericanOption(expiry, strike) {}

// If the option is in the money, the payoff is strike - spot, otherwise, the payoff is zero
double AmericanPutOption::payoff(double spot) const {
    if (spot < _strike) {
        return _strike - spot;
    }
    return 0.0;
}
//