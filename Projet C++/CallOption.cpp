#include "CallOption.h"
#include <algorithm> // pour std::max

using namespace std;

// Constructeur : transmet expiry et strike à EuropeanVanillaOption
CallOption::CallOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike) {}

// Type de l'option : call
EuropeanVanillaOption::optionType CallOption::GetOptionType() const { 
    return optionType::call;
}

// Payoff du call : max(S - K, 0)
double CallOption::payoff(double z) const {
    return max(z - getStrike(), 0.0);
}
