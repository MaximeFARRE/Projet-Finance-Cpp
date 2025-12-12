#pragma once
#include "Option.h"
#include <vector>
#include <algorithm>
#include <stdexcept>

class AsianOption : public Option {
protected:
    std::vector<double> _timeSteps;
    double _strike;

public:
    AsianOption(const std::vector<double>& timeSteps, double strike)
        : Option(timeSteps.empty() ? 0.0 : timeSteps.back()),
        _timeSteps(timeSteps),
        _strike(strike)
    {
        if (timeSteps.empty())
            throw std::invalid_argument("timeSteps cannot be empty");
        if (strike < 0.0)
            throw std::invalid_argument("strike must be non-negative");
    }

    virtual bool isAsianOption() const override { return true; }

    const std::vector<double>& getTimeSteps() const { return _timeSteps; }

    // On override correctement car Option.h contient maintenant payoffPath !
    virtual double payoffPath(const std::vector<double>& path) const override {
        if (path.size() != _timeSteps.size())
            throw std::invalid_argument("path size mismatch");

        double sum = 0.0;
        for (double x : path) sum += x;

        double mean = sum / path.size();
        return payoff(mean);  // payoff(x) sera défini dans les classes filles
    }

    virtual double payoff(double x) const = 0;
};
