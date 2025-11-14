#pragma once

#include "AsianOption.h"
#include <algorithm>

class AsianCallOption : public AsianOption {
public:
    AsianCallOption(const std::vector<double>& timeSteps, double strike)
        : AsianOption(timeSteps, strike) {
    }

    virtual double payoff(double x) const override {
        return std::max(x - _strike, 0.0);
    }
};

