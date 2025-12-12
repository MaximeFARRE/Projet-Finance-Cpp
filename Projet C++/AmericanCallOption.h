#pragma once
#include "AmericanOption.h"

class AmericanCallOption : public AmericanOption {
public:
    AmericanCallOption(double expiry, double strike);

    virtual double payoff(double spot) const override;
};
