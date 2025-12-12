#include "BlackScholesPricer.h"
#include "EuropeanDigitalCallOption.h"
#include "EuropeanDigitalPutOption.h"
#include "CallOption.h"
#include "PutOption.h"
#include <cmath>
#include <stdexcept>
//
BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option,
                                       double asset_price,
                                       double interest_rate,
                                       double volatility)
    : _option(option),
      _assetPrice(asset_price),
      _interestRate(interest_rate),
      _volatility(volatility) {

    if (!_option) {
        throw std::invalid_argument("option pointer is null");
    }
}

double BlackScholesPricer::normalCdf(double x) const {
    // standard normal CDF using erfc
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

double BlackScholesPricer::normalPdf(double x) const {
    const double invSqrt2Pi = 1.0 / std::sqrt(2.0 * 3.141592653589793);
    return invSqrt2Pi * std::exp(-0.5 * x * x);
}

double BlackScholesPricer::operator()() {
    double T = _option->getExpiry();
    double K = _option->_strike; // friend access

    if (T <= 0.0 || _volatility <= 0.0) {
        // simple fallback
        return std::exp(-_interestRate * T) * _option->payoff(_assetPrice);
    }

    double sqrtT = std::sqrt(T);
    double d1 = (std::log(_assetPrice / K)
                + (_interestRate + 0.5 * _volatility * _volatility) * T)
                / (_volatility * sqrtT);
    double d2 = d1 - _volatility * sqrtT;

    bool isDigital = (dynamic_cast<EuropeanDigitalOption*>(_option) != nullptr);
    EuropeanVanillaOption::optionType type = _option->GetOptionType();

    double price = 0.0;

    if (!isDigital) {
        // vanilla call / put
        if (type == EuropeanVanillaOption::call) {
            price = _assetPrice * normalCdf(d1)
                  - K * std::exp(-_interestRate * T) * normalCdf(d2);
        } else {
            price = K * std::exp(-_interestRate * T) * normalCdf(-d2)
                  - _assetPrice * normalCdf(-d1);
        }
    } else {
        // digital call / put
        if (type == EuropeanVanillaOption::call) {
            price = std::exp(-_interestRate * T) * normalCdf(d2);
        } else {
            price = std::exp(-_interestRate * T) * normalCdf(-d2);
        }
    }

    return price;
}

double BlackScholesPricer::delta() {
    double T = _option->getExpiry();
    double K = _option->_strike;

    if (T <= 0.0 || _volatility <= 0.0) {
        return 0.0;
    }

    double sqrtT = std::sqrt(T);
    double d1 = (std::log(_assetPrice / K)
                + (_interestRate + 0.5 * _volatility * _volatility) * T)
                / (_volatility * sqrtT);
    double d2 = d1 - _volatility * sqrtT;

    bool isDigital = (dynamic_cast<EuropeanDigitalOption*>(_option) != nullptr);
    EuropeanVanillaOption::optionType type = _option->GetOptionType();

    double d = 0.0;

    if (!isDigital) {
        if (type == EuropeanVanillaOption::call) {
            d = normalCdf(d1);
        } else {
            d = normalCdf(d1) - 1.0;
        }
    } else {
        double pdf_d2 = normalPdf(d2);
        double factor = std::exp(-_interestRate * T)
                        * pdf_d2
                        / (_assetPrice * _volatility * sqrtT);
        if (type == EuropeanVanillaOption::call) {
            d = factor;
        } else {
            d = -factor;
        }
    }

    return d;
}
