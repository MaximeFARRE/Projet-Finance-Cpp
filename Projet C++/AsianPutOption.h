#pragma once
#include "AsianOption.h"
#include <vector>

using namespace std;

// AsianPutOption is a child class of AsianOption
class AsianPutOption : public AsianOption {
public:
    // Constructor
    AsianPutOption(const vector<double>& timeSteps, double strike);

    // Payoff 
    double payoff(double x) const override;
};
