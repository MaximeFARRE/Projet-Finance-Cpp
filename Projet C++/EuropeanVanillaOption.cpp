#include "EuropeanVanillaOption.h"
#include <stdexcept>

using namespace std;

// Constructor
EuropeanVanillaOption::EuropeanVanillaOption(double expiry, double strike) : Option(expiry), _strike(strike)
{
    if (expiry < 0.0 || strike < 0.0) {
        throw invalid_argument("expiry and strike must be non-negative");
    }
}
//