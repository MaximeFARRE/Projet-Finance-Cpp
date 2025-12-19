#pragma once
#include "AmericanOption.h"

// AmericanPutOption is a child class of AmericanOption
class AmericanPutOption : public AmericanOption {
public:
    // Constructor
    AmericanPutOption(double expiry, double strike);

    // Payoff function
    virtual double payoff(double spot) const override;
};
