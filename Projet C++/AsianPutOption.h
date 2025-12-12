#pragma once

#include "AsianOption.h"
#include <vector>
using namespace std;

// Asian put option class
class AsianPutOption : public AsianOption {
public:
    // Constructor
    AsianPutOption(const vector<double>& timeSteps, double strike);

    // Payoff of the Asian put
    double payoff(double x) const override;
};
