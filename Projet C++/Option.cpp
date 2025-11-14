#include "Option.h"

Option::Option(double expiry)
    : _expiry(expiry)
{
}

double Option::getExpiry() const {
    return _expiry;
}

// Définition de payoffPath DOIT correspondre 100% à la déclaration
double Option::payoffPath(const std::vector<double>& path) const {
    return payoff(path.back());
}
