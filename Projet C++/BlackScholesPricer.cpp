#include "BlackScholesPricer.h"
#include <cmath>
#include <stdexcept>

using namespace std;

// Constant used for the normal pdf: 1 / sqrt(2*pi)
static const double INV_SQRT_2PI = 0.3989422804014327;

// Constructor for vanilla options
BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility)
{
    vanillaOption = option;
    digitalOption = nullptr;

    assetPrice = asset_price;
    interestRate = interest_rate;
    _volatility = volatility;

    if (vanillaOption == nullptr) {
        throw invalid_argument("vanilla option pointer is null");
    }
    if (assetPrice <= 0.0) {
        throw invalid_argument("asset price must be > 0");
    }
    if (_volatility < 0.0) {
        throw invalid_argument("volatility must be >= 0");
    }
}

// Constructor for digital options
BlackScholesPricer::BlackScholesPricer(EuropeanDigitalOption* option, double asset_price, double interest_rate, double volatility)
{
    vanillaOption = nullptr;
    digitalOption = option;

    assetPrice = asset_price;
    interestRate = interest_rate;
    _volatility = volatility;

    if (digitalOption == nullptr) {
        throw invalid_argument("digital option pointer is null");
    }
    if (assetPrice <= 0.0) {
        throw invalid_argument("asset price must be > 0");
    }
    if (_volatility < 0.0) {
        throw invalid_argument("volatility must be >= 0");
    }
}

// Standard normal cdf
double BlackScholesPricer::normal_cdf(double x) {
    return 0.5 * erfc(-x / sqrt(2.0));
}

// Standard normal pdf
double BlackScholesPricer::normal_pdf(double x) {
    return INV_SQRT_2PI * exp(-0.5 * x * x);
}

// Pricing function 
double BlackScholesPricer::operator()() const {
    if (vanillaOption == nullptr && digitalOption == nullptr) {
        throw runtime_error("no option set");
    }

    // Time to maturity
    double T = (vanillaOption != nullptr) ? vanillaOption->getExpiry() : digitalOption->getExpiry();

    if (T < 0.0) {
        throw invalid_argument("expiry must be >= 0");
    }

    // If maturity is now, price is just the payoff
    if (T == 0.0) {
        if (vanillaOption != nullptr) {
            return vanillaOption->payoff(assetPrice);
        }
        return digitalOption->payoff(assetPrice);
    }

    // If volatility is 0, the path is deterministic
    if (_volatility == 0.0) {
        double discountFactor = exp(-interestRate * T);
        double ST = assetPrice * exp(interestRate * T);

        if (vanillaOption != nullptr) {
            return discountFactor * vanillaOption->payoff(ST);
        }
        return discountFactor * digitalOption->payoff(ST);
    }

    // Common values
    double sqrtT = sqrt(T);
    double sigmaSqrtT = _volatility * sqrtT;
    double discountFactor = exp(-interestRate * T);

    // Vanilla option case
    if (vanillaOption != nullptr) {
        double K = vanillaOption->_strike;

        if (K <= 0.0) {
            throw invalid_argument("strike must be > 0");
        }

        double d1 = (log(assetPrice / K) + (interestRate + 0.5 * _volatility * _volatility) * T) / sigmaSqrtT;
        double d2 = d1 - sigmaSqrtT;

        if (vanillaOption->GetOptionType() == EuropeanVanillaOption::call) {
            return assetPrice * normal_cdf(d1) - K * discountFactor * normal_cdf(d2);
        }
        return K * discountFactor * normal_cdf(-d2) - assetPrice * normal_cdf(-d1);
    }

    // Digital option case
    double K = digitalOption->_strike;

    if (K <= 0.0) {
        throw invalid_argument("strike must be > 0");
    }

    double d2 = (log(assetPrice / K) + (interestRate - 0.5 * _volatility * _volatility) * T) / sigmaSqrtT;

    if (digitalOption->GetOptionType() == EuropeanDigitalOption::call) {
        return discountFactor * normal_cdf(d2);
    }
    return discountFactor * normal_cdf(-d2);
}

// Delta function (vanilla or digital)
double BlackScholesPricer::delta() const {
    if (vanillaOption == nullptr && digitalOption == nullptr) {
        throw runtime_error("no option set");
    }

    double T = (vanillaOption != nullptr) ? vanillaOption->getExpiry() : digitalOption->getExpiry();

    if (T < 0.0) {
        throw invalid_argument("expiry must be >= 0");
    }
    if (T == 0.0) {
        throw runtime_error("delta undefined at T=0");
    }
    if (_volatility == 0.0) {
        throw runtime_error("delta undefined for sigma=0");
    }

    double sqrtT = sqrt(T);
    double discountFactor = exp(-interestRate * T);

    // Vanilla option delta
    if (vanillaOption != nullptr) {
        double K = vanillaOption->_strike;

        if (K <= 0.0) {
            throw invalid_argument("strike must be > 0");
        }

        double d1 = (log(assetPrice / K) + (interestRate + 0.5 * _volatility * _volatility) * T) / (_volatility * sqrtT);

        if (vanillaOption->GetOptionType() == EuropeanVanillaOption::call) {
            return normal_cdf(d1);
        }
        return normal_cdf(d1) - 1.0;
    }

    // Digital option delta
    double K = digitalOption->_strike;

    if (K <= 0.0) {
        throw invalid_argument("strike must be > 0");
    }

    double d2 = (log(assetPrice / K) + (interestRate - 0.5 * _volatility * _volatility) * T) / (_volatility * sqrtT);

    double digitalDelta = discountFactor * normal_pdf(d2) / (assetPrice * _volatility * sqrtT);

    if (digitalOption->GetOptionType() == EuropeanDigitalOption::call) {
        return digitalDelta;
    }
    return -digitalDelta;
}
