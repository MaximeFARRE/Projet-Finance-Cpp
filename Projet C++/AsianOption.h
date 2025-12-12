#pragma once
#include "Option.h"
#include <vector>

class AsianOption : public Option {
protected:
    std::vector<double> _timeSteps;

public:
    // constructor: takes all time steps t1,...,tm
    AsianOption(const std::vector<double>& timeSteps);

    // getter on time steps
    const std::vector<double>& getTimeSteps() const;

    // asian flag
    virtual bool isAsianOption() const override;

    // payoffPath for Asian: use average of the path
    virtual double payoffPath(const std::vector<double>& pricePath) override;
};
//