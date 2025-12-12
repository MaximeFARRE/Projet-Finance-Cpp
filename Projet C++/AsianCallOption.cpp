#include "AsianCallOption.h"
#include <algorithm>   
using namespace std;

// we call the constructor of the base class AsianOption
AsianCallOption::AsianCallOption(const vector<double>& timeSteps, double strike)
    : AsianOption(timeSteps, strike) // initialize base class
{
}

// Payoff du call asiatique : max(moyenne - strike, 0)
double AsianCallOption::payoff(double x) const {
    return max(x - _strike, 0.0);
}
