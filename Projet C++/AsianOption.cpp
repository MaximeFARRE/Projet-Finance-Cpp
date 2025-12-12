#include "AsianOption.h"
#include <numeric>     

// Constructor
AsianOption::AsianOption(const std::vector<double>& timeSteps, double strike)
    : Option(timeSteps.empty() ? 0.0 : timeSteps.back()),
      _timeSteps(timeSteps),
      _strike(strike)
{
    if (timeSteps.empty()) {
        throw std::invalid_argument("invalid timeSteps empty");
    }
    if (strike < 0.0) {
        throw std::invalid_argument("strike negative not valid");
    }
}

// Computes payoff from the whole path by averaging the values
double AsianOption::payoffPath(const std::vector<double>& path) const {
    if (path.size() != _timeSteps.size()) {
        throw std::invalid_argument("path invalid");
    }

    // Compute the mean value of the price path
    double sum = std::accumulate(path.begin(), path.end(), 0.0);
    double mean = sum / path.size();

    return payoff(mean);
}
