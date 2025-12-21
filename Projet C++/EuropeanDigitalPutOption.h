#pragma once
#include "EuropeanDigitalOption.h"



class EuropeanDigitalPutOption : public EuropeanDigitalOption {
public:
    // Constructor
    EuropeanDigitalPutOption(double expiry, double strike);

    // Payoff function
    double payoff(double spot) const override;

    // Return the option type
    optionType GetOptionType() const override;
};
