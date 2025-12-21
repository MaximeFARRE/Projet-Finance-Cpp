#pragma once
#include "EuropeanDigitalOption.h"

// EuropeanDigitalCallOption is a child class of EuropeanDigitalOption
class EuropeanDigitalCallOption : public EuropeanDigitalOption {
public:
    // Constructor
    EuropeanDigitalCallOption(double expiry, double strike);
    // Payoff function
    double payoff(double spot) const override;
    // Return the option type
    optionType GetOptionType() const override;
};
//