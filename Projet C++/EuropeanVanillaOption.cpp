#include "EuropeanVanillaOption.h"
#include <stdexcept>

using namespace std;

// Constructor: checks that expiry and strike are >= 0
EuropeanVanillaOption::EuropeanVanillaOption(double expiry, double strike) : Option(expiry), _strike(strike)
{
    if (expiry < 0.0 || strike < 0.0) {
        throw std::invalid_argument("expiry and strike must be non-negative");
    }
}
