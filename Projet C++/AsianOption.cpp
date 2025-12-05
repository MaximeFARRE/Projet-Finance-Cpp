#include "AsianOption.h"
#include <stdexcept>

AsianOption::AsianOption(const std::vector<double>& timeSteps)
    : Option(timeSteps.empty() ? 0.0 : timeSteps.back()),
      _timeSteps(timeSteps) {

    if (_timeSteps.empty()) {
        throw std::invalid_argument("time step vector must not be empty");
    }
}

const std::vector<double>& AsianOption::getTimeSteps() const {
    return _timeSteps;
}

bool AsianOption::isAsianOption() const {
    return true;
}

double AsianOption::payoffPath(const std::vector<double>& pricePath) {
    // average all prices then use payoff(double)
    if (pricePath.empty()) {
        return 0.0;
    }

    double sum = 0.0;
    for (size_t i = 0; i < pricePath.size(); ++i) {
        sum += pricePath[i];
    }
    double avg = sum / static_cast<double>(pricePath.size());
    return payoff(avg);
}
