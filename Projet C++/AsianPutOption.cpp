#include "AsianPutOption.h"

AsianPutOption::AsianPutOption(const std::vector<double>& timeSteps, double strike)
    : AsianOption(timeSteps), _strike(strike) {
}

double AsianPutOption::payoff(double x) const {
    if (x < _strike) {
        return _strike - x;
    }
    return 0.0;
}
