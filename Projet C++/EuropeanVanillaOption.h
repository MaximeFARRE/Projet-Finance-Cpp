#pragma once

#include "Option.h"

// Classe abstraite pour les options européennes vanilles (call/put)
class EuropeanVanillaOption : public Option {
public:
    // Type d'option : call ou put
    enum optionType { call, put };

private:
    double _strike;  // prix d'exercice K
    friend class BlackScholesPricer;


public:
    // Constructeur : initialise expiry (T) et strike (K)
    EuropeanVanillaOption(double expiry, double strike);

    // Getter sur le strike
    double getStrike() const { return _strike; }

    // Chaque option devra indiquer si elle est call ou put
    virtual optionType GetOptionType() const = 0;
};
