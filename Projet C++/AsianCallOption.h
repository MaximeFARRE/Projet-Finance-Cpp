#pragma once
#include <vector>      
#include "AsianOption.h"

// call class for Asian options
class AsianCallOption : public AsianOption {
public:
    // constructor
    AsianCallOption(const std::vector<double>& timeSteps, double strike);

    // Payoff 
    double payoff(double x) const override;
};
