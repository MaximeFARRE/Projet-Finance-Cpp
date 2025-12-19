#include "Option.h"

// Constructeur de la classe Option
// Initialise la maturité de l'option
Option::Option(double expiry): _expiry(expiry)
{}

// Retourne la maturité (date d'expiration) de l'option
double Option::getExpiry() const {
    return _expiry;
}

// Calcule le payoff de l'option à partir d'un chemin de prix
// path : vecteur contenant l'évolution du prix du sous-jacent
// Le payoff dépend uniquement du prix final (à maturité)
double Option::payoffPath(const std::vector<double>& path) const {
    return payoff(path.back());
}
