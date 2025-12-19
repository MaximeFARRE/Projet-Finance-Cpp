#pragma once
#include "AmericanOption.h"

// AmericanCallOption is a child class of AmericanOption
class AmericanCallOption : public AmericanOption {
public:
    // Constructor
    AmericanCallOption(double expiry, double strike);

    // Payoff function 
    virtual double payoff(double spot) const override;
};
