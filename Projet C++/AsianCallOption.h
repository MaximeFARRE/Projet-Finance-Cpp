#pragma once
#include <vector>      
#include "AsianOption.h"
using namespace std;

// AsianCallOption is a child class of AsianOption
class AsianCallOption : public AsianOption {
private:
    double _strike;

public:
    // constructor
    AsianCallOption(const vector<double>& timeSteps, double strike);

    // Payoff 
    double payoff(double x) const override;
};
