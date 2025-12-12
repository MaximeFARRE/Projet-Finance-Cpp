#pragma once
#include "AsianOption.h"

class AsianCallOption : public AsianOption {
private:
    double _strike;

public:
    AsianCallOption(const std::vector<double>& timeSteps, double strike);

    virtual double payoff(double x) const override;
};
//