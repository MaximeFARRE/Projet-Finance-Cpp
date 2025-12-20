#pragma once

#include <vector>

// Abstract base class for all options
class Option {
protected:
    double _expiry;   // Maturity T

public:
    explicit Option(double expiry);

    double getExpiry() const;

    // Payoff h(z) of the option
    virtual double payoff(double z) const = 0;

    // By default: not Asian, not American
    virtual bool isAsianOption() const { return false; }
    virtual bool isAmericanOption() const { return false; }

    // Payoff computed from a path (default: payoff at the last point)
    virtual double payoffPath(const std::vector<double>& path) const;

    virtual ~Option() = default;
};
