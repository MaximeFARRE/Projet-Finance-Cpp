#pragma once
#include "AmericanOption.h"

class AmericanPutOption : public AmericanOption {
public:
    AmericanPutOption(double expiry, double strike);

    virtual double payoff(double spot) const override;
};
//