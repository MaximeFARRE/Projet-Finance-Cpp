#pragma once
#include "Option.h"

// AmericanOption is a child class of Option
class AmericanOption : public Option {
protected:
    double _strike; 

public:
    // constructor
    AmericanOption(double expiry, double strike);

    // Indicates that this option is an American option or not
    virtual bool isAmericanOption() const override;

    // Getter
    double getStrike() const;
};
//