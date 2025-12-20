#include "Option.h"

// Constructor of the Option class
// Initializes the option's maturity
Option::Option(double expiry): _expiry(expiry)
{}

// Returns the option's maturity (expiration date)
double Option::getExpiry() const {
    return _expiry;
}

// Calculates the option's payoff from a price path
// path: vector containing the evolution of the underlying asset's price
// The payoff depends only on the final price (at maturity)
double Option::payoffPath(const std::vector<double>& path) const {
    return payoff(path.back());
}
