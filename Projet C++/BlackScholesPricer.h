#pragma once

#include "EuropeanVanillaOption.h"
#include "EuropeanDigitalOption.h"

// Black-Scholes pricer for European vanilla and digital options
class BlackScholesPricer {
private:
    EuropeanVanillaOption* vanillaOption;
    EuropeanDigitalOption* digitalOption;

    double assetPrice;
    double interestRate;
    double _volatility;

public:
    // Constructor for vanilla options
    BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility);

    // Constructor for digital options
    BlackScholesPricer(EuropeanDigitalOption* option, double asset_price, double interest_rate, double volatility);

    // Return the option price
    double operator()() const;

    // Return the delta of the option
    double delta() const;

    // Normal distribution functions
    static double normal_cdf(double x);
    static double normal_pdf(double x);
};
