#include "CallOption.h"
#include <algorithm> // for std::max

using namespace std;

// Constructor: passes expiry and strike to EuropeanVanillaOption
CallOption::CallOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike) {}

// Option type : put
EuropeanVanillaOption::optionType CallOption::GetOptionType() const { 
    return optionType::call;
}

// Call payoff : max(S - K, 0)
double CallOption::payoff(double z) const {
    return max(z - getStrike(), 0.0);
}
