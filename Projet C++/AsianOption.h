#pragma once
#include "Option.h"
#include <vector>
#include <stdexcept>
using namespace std;

// Base class for Asian options 
class AsianOption : public Option {
protected:
    vector<double> _timeSteps;  // Time points where the asset is observed
    double _strike;                  // Strike price

public:
    // Constructor
    AsianOption(const vector<double>& timeSteps, double strike);

    // isAsianOption override
    bool isAsianOption() const override { return true; }

    // time steps getter
    const vector<double>& getTimeSteps() const { return _timeSteps; }

    // Computes payoff on a path
    double payoffPath(const vector<double>& path) const override;

    // Payoff function
    virtual double payoff(double x) const = 0;
};
//