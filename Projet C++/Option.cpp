#include "Option.h"
#include <stdexcept>

Option::Option(double expiry) : _expiry(expiry) {
    if (expiry < 0.0) {
        throw std::invalid_argument("expiry must be non-negative");
    }
}

double Option::getExpiry() const {
    return _expiry;
}
//
double Option::payoffPath(const std::vector<double>& pricePath) {
    // default: just use last price and call payoff(double)
    if (pricePath.empty()) {
        return 0.0;
    }
    double lastPrice = pricePath.back();
    return payoff(lastPrice);
}

bool Option::isAsianOption() const {
    return false;
}

bool Option::isAmericanOption() const {
    return false;
}
