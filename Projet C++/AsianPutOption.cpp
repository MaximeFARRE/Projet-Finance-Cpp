#include "AsianPutOption.h"
#include <algorithm> 
using namespace std;

// Constructor
AsianPutOption::AsianPutOption(const vector<double>& timeSteps, double strike)
    : AsianOption(timeSteps, strike)
{
}

// Payoff of an Asian put
double AsianPutOption::payoff(double x) const {
    return max(_strike - x, 0.0);
}
