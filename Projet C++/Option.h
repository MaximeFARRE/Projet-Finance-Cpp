#pragma once
#include <vector>
#include <cmath>

// Abstract base class for all options
class Option {
protected:
    double _expiry;   

public:
    // constructor
    Option(double expiry);
    // destructeur
    virtual ~Option() {}

    // getter 
    double getExpiry() const;

    // Payoff 
    virtual double payoff(double z) const = 0;

    // By default :  not Asian and not American
    virtual bool isAsianOption() const { return false; }
    virtual bool isAmericanOption() const { return false; }

    // Payoff 
    virtual double payoffPath(const std::vector<double>& path) const;

    
};
