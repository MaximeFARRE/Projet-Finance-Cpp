#pragma once
#include <vector>

class Option {
private:
    double _expiry; // maturity time T

public:
    // constructor: set expiry
    Option(double expiry);
    // destructeur
    virtual ~Option() {}

    // getter on expiry
    double getExpiry() const;

    // basic payoff with only final price
    virtual double payoff(double spot) const = 0;

    // payoff with a full path (for path dependent options)
    // default: use only the last price in the path
    virtual double payoffPath(const std::vector<double>& pricePath);

    // flags for option type
    virtual bool isAsianOption() const;
    virtual bool isAmericanOption() const;

    
};
