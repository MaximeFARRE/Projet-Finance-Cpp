#include "Option.h"
#include <stdexcept>

using namespace std;

Option::Option(double expiry) : _expiry(expiry) {
    if (expiry < 0.0) {
        throw std::invalid_argument("expiry must be non-negative");
    }
}

double Option::getExpiry() const {
    return _expiry;
}

double Option::payoffPath(const std::vector<double>& pricePath) const {
    if (pricePath.empty()) {
        throw invalid_argument("pricePath is empty");
    }
    return payoff(pricePath.back());
}

bool Option::isAsianOption() const {
    return false;
}

bool Option::isAmericanOption() const {
    return false;
}
