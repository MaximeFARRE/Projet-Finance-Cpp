#include "AmericanCallOption.h"

using namespace std;

//constructor
AmericanCallOption::AmericanCallOption(double expiry, double strike): AmericanOption(expiry, strike) {}

// If the option is in the money, the payoff is spot - strike, otherwise, the payoff is 0
double AmericanCallOption::payoff(double spot) const {
    if (spot > _strike) {
        return spot - _strike;
    }
    return 0.0;
}

