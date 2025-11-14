#pragma once

#include "AmericanOption.h"
#include <algorithm>

class AmericanPutOption : public AmericanOption {
public:
    AmericanPutOption(double expiry, double strike)
        : AmericanOption(expiry, strike) {
    }

    virtual double payoff(double S) const override {
        return std::max(_strike - S, 0.0);
    }
};
