#include "AsianCallOption.h"
#include <algorithm>   

// we call the constructor of the base class AsianOption
AsianCallOption::AsianCallOption(const std::vector<double>& timeSteps, double strike)
    : AsianOption(timeSteps, strike) // initialize base class
{
}

// Payoff du call asiatique : max(moyenne - strike, 0)
double AsianCallOption::payoff(double x) const {
    return std::max(x - _strike, 0.0);
}
