#pragma once

#include "Option.h"
#include <stdexcept>

// Classe abstraite pour options américaines (call/put)
class AmericanOption : public Option {
protected:
    double _strike;

public:
    AmericanOption(double expiry, double strike)
        : Option(expiry), _strike(strike)
    {
        if (expiry < 0.0 || strike < 0.0) {
            throw std::invalid_argument("expiry and strike must be non-negative");
        }
    }

    double getStrike() const { return _strike; }

    // C'est une option américaine
    virtual bool isAmericanOption() const override { return true; }

    // Payoff en fonction de S (intrinsic value) → implémenté dans les classes filles
    virtual double payoff(double S) const override = 0;
};

