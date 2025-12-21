#include "CallOption.h"
#include <algorithm> 

using namespace std;

// Constructor
CallOption::CallOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike) {}

// Type
EuropeanVanillaOption::optionType CallOption::GetOptionType() const { 
    return optionType::call;
}

//  payoff
double CallOption::payoff(double z) const {
    return max(z - getStrike(), 0.0);
}
