#include "EuropeanVanillaOption.h"
#include <stdexcept>

using namespace std;

// Constructeur : vérifie que expiry et strike sont >= 0
EuropeanVanillaOption::EuropeanVanillaOption(double expiry, double strike) : Option(expiry), _strike(strike)
{
    if (expiry < 0.0 || strike < 0.0) {
        throw std::invalid_argument("expiry and strike must be non-negative");
    }
}
