#pragma once
#include "AsianOption.h"

class AsianPutOption : public AsianOption {
private:
    double _strike;

public:
    AsianPutOption(const std::vector<double>& timeSteps, double strike);

    virtual double payoff(double x) const override;
};
