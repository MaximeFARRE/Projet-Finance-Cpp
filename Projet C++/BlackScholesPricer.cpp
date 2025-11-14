#include "BlackScholesPricer.h"
#include <cmath>

// CDF de la loi normale standard N(x)
static double norm_cdf(double x)
{
    // N(x) = 0.5 * erfc(-x / sqrt(2))
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

// Densité (pdf) de la loi normale standard
static double norm_pdf(double x)
{
    const double inv_sqrt_2pi = 1.0 / std::sqrt(2.0 * 3.14159265358979323846);
    return inv_sqrt_2pi * std::exp(-0.5 * x * x);
}

// Constructeur pour options vanilles
BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option,
    double asset_price,
    double interest_rate,
    double volatility)
    : _family(OptionFamily::Vanilla),
    _vanillaOption(option),
    _digitalOption(nullptr),
    _S(asset_price),
    _r(interest_rate),
    _sigma(volatility)
{
}

// Constructeur pour options digitales
BlackScholesPricer::BlackScholesPricer(EuropeanDigitalOption* option,
    double asset_price,
    double interest_rate,
    double volatility)
    : _family(OptionFamily::Digital),
    _vanillaOption(nullptr),
    _digitalOption(option),
    _S(asset_price),
    _r(interest_rate),
    _sigma(volatility)
{
}

// Prix Black-Scholes de l'option (vanille ou digitale)
double BlackScholesPricer::operator()()
{
    double T = 0.0;
    double K = 0.0;

    if (_family == OptionFamily::Vanilla) {
        T = _vanillaOption->getExpiry();
        K = _vanillaOption->getStrike();
    }
    else {
        T = _digitalOption->getExpiry();
        K = _digitalOption->getStrike();
    }

    double sqrtT = std::sqrt(T);
    double d1 = (std::log(_S / K) + (_r + 0.5 * _sigma * _sigma) * T) /
        (_sigma * sqrtT);
    double d2 = d1 - _sigma * sqrtT;

    if (_family == OptionFamily::Vanilla) {
        // ==== VANILLA CALL / PUT ====
        if (_vanillaOption->GetOptionType() == EuropeanVanillaOption::call) {
            // Call : C = S N(d1) - K e^{-rT} N(d2)
            return _S * norm_cdf(d1) - K * std::exp(-_r * T) * norm_cdf(d2);
        }
        else {
            // Put : P = K e^{-rT} N(-d2) - S N(-d1)
            return K * std::exp(-_r * T) * norm_cdf(-d2)
                - _S * norm_cdf(-d1);
        }
    }
    else {
        // ==== DIGITAL CALL / PUT ====
        // Attention : on suppose une option cash-or-nothing qui paye 1 si condition satisfaite
        if (_digitalOption->GetOptionType() == EuropeanDigitalOption::call) {
            // Digital call : prix = e^{-rT} N(d2)
            return std::exp(-_r * T) * norm_cdf(d2);
        }
        else {
            // Digital put : prix = e^{-rT} N(-d2)
            return std::exp(-_r * T) * norm_cdf(-d2);
        }
    }
}

// Delta de l'option (vanille ou digitale)
double BlackScholesPricer::delta()
{
    double T = 0.0;
    double K = 0.0;

    if (_family == OptionFamily::Vanilla) {
        T = _vanillaOption->getExpiry();
        K = _vanillaOption->getStrike();
    }
    else {
        T = _digitalOption->getExpiry();
        K = _digitalOption->getStrike();
    }

    double sqrtT = std::sqrt(T);
    double d1 = (std::log(_S / K) + (_r + 0.5 * _sigma * _sigma) * T) /
        (_sigma * sqrtT);
    double d2 = d1 - _sigma * sqrtT;

    if (_family == OptionFamily::Vanilla) {
        // ==== VANILLA CALL / PUT ====
        if (_vanillaOption->GetOptionType() == EuropeanVanillaOption::call) {
            // Delta call : N(d1)
            return norm_cdf(d1);
        }
        else {
            // Delta put : N(d1) - 1
            return norm_cdf(d1) - 1.0;
        }
    }
    else {
        // ==== DIGITAL CALL / PUT ====
        // Formule pour une digital cash-or-nothing :
        // Delta = e^{-rT} * pdf(d2) / (S * sigma * sqrt(T)) pour le call
        double common = std::exp(-_r * T) * norm_pdf(d2) /
            (_S * _sigma * sqrtT);

        if (_digitalOption->GetOptionType() == EuropeanDigitalOption::call) {
            return common;          // digital call
        }
        else {
            return -common;         // digital put
        }
    }
}
