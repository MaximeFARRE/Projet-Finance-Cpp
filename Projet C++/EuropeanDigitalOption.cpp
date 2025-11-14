#include "EuropeanDigitalOption.h"
#include <stdexcept>

EuropeanDigitalOption::EuropeanDigitalOption(double expiry, double strike)
    : Option(expiry), _strike(strike)
{
    if (expiry < 0.0 || strike < 0.0) {
        throw std::invalid_argument("expiry and strike must be non-negative");
    }
}
