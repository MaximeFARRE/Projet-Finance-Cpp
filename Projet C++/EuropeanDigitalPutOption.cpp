#include "EuropeanDigitalPutOption.h"

using namespace std;

// Constructor for a European digital put option
EuropeanDigitalPutOption::EuropeanDigitalPutOption(double expiry, double strike) : EuropeanDigitalOption(expiry, strike){}


// If the option is in the money, the payoff is 1, otherwise, the payoff is 0
double EuropeanDigitalPutOption::payoff(double spot) const {
    if (spot <= _strike) {
        return 1.0;
    }
    return 0.0;
}

// Return the type of the option
EuropeanDigitalOption::optionType EuropeanDigitalPutOption::GetOptionType() const {
    return put;
}
