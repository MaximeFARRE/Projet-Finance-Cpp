#pragma once

#include "AmericanOption.h"
#include <algorithm>

class AmericanCallOption : public AmericanOption {
public:
    AmericanCallOption(double expiry, double strike)
        : AmericanOption(expiry, strike) {
    }

    virtual double payoff(double S) const override {
        return std::max(S - _strike, 0.0);
    }
};

