#pragma once

#include "EuropeanVanillaOption.h"
#include "EuropeanDigitalOption.h"
#include <cmath>

class BlackScholesPricer {
private:
    // Pour savoir quel type d'option on price
    enum class OptionFamily { Vanilla, Digital };

    OptionFamily _family;

    EuropeanVanillaOption* _vanillaOption;  // nullptr si digital
    EuropeanDigitalOption* _digitalOption;  // nullptr si vanilla

    double _S;      // prix spot S0
    double _r;      // taux sans risque
    double _sigma;  // volatilité

public:
    // Constructeur pour options vanilles (call/put classiques)
    BlackScholesPricer(EuropeanVanillaOption* option,
        double asset_price,
        double interest_rate,
        double volatility);

    // Surcharge pour options digitales
    BlackScholesPricer(EuropeanDigitalOption* option,
        double asset_price,
        double interest_rate,
        double volatility);

    // Renvoie le prix (formule de Black-Scholes)
    double operator()();

    // Renvoie le Delta de l’option
    double delta();
};
