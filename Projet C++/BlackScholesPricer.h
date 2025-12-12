#pragma once
#include "EuropeanVanillaOption.h"
#include "EuropeanDigitalOption.h"

class BlackScholesPricer {
private:
    EuropeanVanillaOption* _option;
    double _assetPrice;
    double _interestRate;
    double _volatility;

    double normalCdf(double x) const;
    double normalPdf(double x) const;

public:
    BlackScholesPricer(EuropeanVanillaOption* option,
                       double asset_price,
                       double interest_rate,
                       double volatility);

    // price
    double operator()();

    // delta
    double delta();
};
