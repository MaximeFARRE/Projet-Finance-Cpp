#include "EuropeanDigitalCallOption.h"

using namespace std;

// Constructor 
EuropeanDigitalCallOption::EuropeanDigitalCallOption(double expiry, double strike) : EuropeanDigitalOption(expiry, strike){}

// If the option is in the money, the payoff is 1 otherwise the payoff is 0
double EuropeanDigitalCallOption::payoff(double spot) const {
    if (spot >= _strike) {
        return 1.0;
    }
    return 0.0;
}

// Return the type of the option
EuropeanDigitalOption::optionType EuropeanDigitalCallOption::GetOptionType() const {
    return call;
}
//