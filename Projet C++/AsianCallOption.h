#pragma once
#include <vector>      
#include "AsianOption.h"
using namespace std;

// call class for Asian options
class AsianCallOption : public AsianOption {
public:
    // constructor
    AsianCallOption(const vector<double>& timeSteps, double strike);

    // Payoff 
    double payoff(double x) const override;
};
