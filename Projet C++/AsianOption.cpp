#include "AsianOption.h"
#include <numeric>     
using namespace std;

// Constructor
AsianOption::AsianOption(const vector<double>& timeSteps, double strike) : Option(timeSteps.empty() ? 0.0 : timeSteps.back()), _timeSteps(timeSteps), _strike(strike)
{
    if (timeSteps.empty()) {
        throw invalid_argument("invalid timeSteps empty");
    }
    if (strike < 0.0) {
        throw invalid_argument("strike negative not valid");
    }

    // check that timeSteps are increasing
    for (size_t i = 1; i < _timeSteps.size(); ++i) {
        if (_timeSteps[i] <= _timeSteps[i - 1]) {
            throw invalid_argument("timeSteps must be increasing");
        }
    }
}


// Computes payoff from the whole path by averaging the values
double AsianOption::payoffPath(const vector<double>& path) const {
    if (path.size() != _timeSteps.size()) {
        throw invalid_argument("path invalid");
    }
    double sum = 0.0;
    for (size_t i = 0; i < path.size(); ++i) {
        if (!isfinite(path[i]) || path[i] < 0.0) {
            throw invalid_argument("path contains invalid value");
        }
        sum += path[i];
    }

    double mean = sum / static_cast<double>(path.size());
    return payoff(mean);
}
