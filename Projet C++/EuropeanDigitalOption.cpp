#include "EuropeanDigitalOption.h"

#include "EuropeanDigitalOption.h"
#include <stdexcept>

using namespace std;

EuropeanDigitalOption::EuropeanDigitalOption(double expiry, double strike) : Option(expiry), _strike(strike)
{
    if (strike < 0.0) {
        throw invalid_argument("strike must be non negative");
    }
}

double EuropeanDigitalOption::getStrike() const {
    return _strike;
}