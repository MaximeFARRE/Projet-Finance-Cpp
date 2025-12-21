#include "Option.h"
#include <stdexcept>

// Constructor 
Option::Option(double expiry): _expiry(expiry) {}

// Getter
double Option::getExpiry() const {
    return _expiry;
}

// payoff 
double Option::payoffPath(const std::vector<double>& path) const {
    if (path.empty()) {
        return 0.0;
    }
    return payoff(path.back());
}