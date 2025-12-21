#include "AsianOption.h"
#include <numeric>     
using namespace std;

// Constructor
AsianOption::AsianOption(const vector<double>& timeSteps, double strike)
    : Option(timeSteps.empty() ? 0.0 : timeSteps.back()),
      _timeSteps(timeSteps),
      _strike(strike)
{
    if (timeSteps.empty()) {
        throw invalid_argument("invalid timeSteps empty");
    }
    if (strike < 0.0) {
        throw invalid_argument("strike negative not valid");
    }
}

// Computes payoff from the whole path by averaging the values
double AsianOption::payoffPath(const vector<double>& path) const {
    if (path.size() != _timeSteps.size()) {
        throw invalid_argument("path invalid");
    }

    // Compute the mean value of the price path
    double sum = accumulate(path.begin(), path.end(), 0.0);
    double mean = sum / path.size();

    return payoff(mean);
}
