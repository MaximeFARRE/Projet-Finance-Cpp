#pragma once

#include "Option.h"

// Classe abstraite pour les options digitales européennes
// (paye 1 ou 0 à l'échéance)
class EuropeanDigitalOption : public Option {
public:
    // Type d'option : call ou put
    enum OptionType { call, put };

private:
    double _strike;  // prix d'exercice K

public:
    // Constructeur : initialise expiry et strike
    EuropeanDigitalOption(double expiry, double strike);

    // Getter sur le strike
    double getStrike() const { return _strike; }

    // Chaque option devra indiquer si elle est call ou put
    virtual OptionType GetOptionType() const = 0;
};
