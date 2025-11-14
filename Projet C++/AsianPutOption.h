#pragma once

#include "AsianOption.h"
#include <algorithm>

class AsianPutOption : public AsianOption {
public:
    AsianPutOption(const std::vector<double>& timeSteps, double strike)
        : AsianOption(timeSteps, strike) {
    }

    virtual double payoff(double x) const override {
        return std::max(_strike - x, 0.0);
    }
};
