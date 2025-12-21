#pragma once
#include "Option.h"
#include <vector>
#include <stdexcept>
using namespace std;

// AsianOption is a child class of Option
class AsianOption : public Option {
protected:
    vector<double> _timeSteps;  
    double _strike;                  

public:
    //Constructor
    AsianOption(const vector<double>& timeSteps, double strike);

    bool isAsianOption() const override { return true; }

    // getter
    const vector<double>& getTimeSteps() const { return _timeSteps; }


    double payoffPath(const vector<double>& path) const override;

    // Payoff function
    virtual double payoff(double x) const = 0;
};
//