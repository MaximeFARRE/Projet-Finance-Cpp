#pragma once
#include "Option.h"

class AmericanOption : public Option {
protected:
    double _strike; // strike is protected so children can use it

public:
    AmericanOption(double expiry, double strike);

    virtual bool isAmericanOption() const override;
};
//