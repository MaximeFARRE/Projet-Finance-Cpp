#include "AsianPutOption.h"
#include <algorithm>

using namespace std;

// Constructor: call base class constructor
AsianPutOption::AsianPutOption(const vector<double>& timeSteps, double strike) : AsianOption(timeSteps, strike) {}

// Payoff 
double AsianPutOption::payoff(double x) const {
    return max(_strike - x, 0.0);
}
