#include "AmericanOption.h"
#include <stdexcept>

AmericanOption::AmericanOption(double expiry, double strike)
    : Option(expiry), _strike(strike) {

    if (expiry < 0.0 || strike < 0.0) {
        throw std::invalid_argument("expiry and strike must be non-negative");
    }
}

bool AmericanOption::isAmericanOption() const {
    return true;
}
//