#include "AsianCallOption.h"
#include <algorithm>   
using namespace std;

// constructor 
AsianCallOption::AsianCallOption(const vector<double>& timeSteps, double strike) : AsianOption(timeSteps, strike) {}

// Payoff function
double AsianCallOption::payoff(double x) const {
    return max(x - _strike, 0.0);
}
