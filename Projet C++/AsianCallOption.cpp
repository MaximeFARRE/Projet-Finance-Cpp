#include "AsianCallOption.h"

AsianCallOption::AsianCallOption(const std::vector<double>& timeSteps, double strike)
    : AsianOption(timeSteps), _strike(strike) {
}

double AsianCallOption::payoff(double x) const {
    if (x > _strike) {
        return x - _strike;
    }
    return 0.0;
}
