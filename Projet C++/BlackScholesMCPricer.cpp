#include "BlackScholesMCPricer.h"
#include "AsianOption.h"
#include "MT.h"

#include <cmath>
#include <iostream>

//  CONSTRUCTOR
// 
BlackScholesMCPricer::BlackScholesMCPricer(Option* option,
                                           double initial_price,
                                           double interest_rate,
                                           double volatility)
    : _option(option),
      _S0(initial_price),
      _r(interest_rate),
      _sigma(volatility),
      _nbPaths(0),
      _sumPayoff(0.0),
      _sumPayoffSquared(0.0)
{
    if (_option == nullptr)
    {
        std::cout << "Error: option = nullptr in BlackScholesMCPricer" << std::endl;
        return;
    }

    // if it is an Asian option, we get all time steps
    if (_option->isAsianOption())
    {
        AsianOption* asian = static_cast<AsianOption*>(_option);
        _timeSteps = asian->getTimeSteps();
    }
    else
    {
        // European option: only maturity T
        double T = _option->getExpiry();
        _timeSteps.clear();
        _timeSteps.push_back(T);
    }
}

//  SIMULATE ONE PATH

void BlackScholesMCPricer::simulateOnePath()
{
    if (_option == nullptr) return;

    double S = _S0;      // price at start
    double prev_t = 0.0; // previous time

    std::vector<double> path;
    path.reserve(_timeSteps.size());

    for (std::size_t i = 0; i < _timeSteps.size(); ++i)
    {
        double t = _timeSteps[i];
        double dt = t - prev_t;

        if (dt > 0.0)
        {
            // draw standard normal
            double Z = MT::rand_norm();

            // drift and diffusion
            double drift = (_r - 0.5 * _sigma * _sigma) * dt;
            double diffusion = _sigma * std::sqrt(dt) * Z;

            // log-normal S_t
            S = S * std::exp(drift + diffusion);
        }

        path.push_back(S);
        prev_t = t;
    }

    // payoff from the path
    double payoff = _option->payoffPath(path);

    // update stats (undiscounted payoff)
    _nbPaths += 1;
    _sumPayoff += payoff;
    _sumPayoffSquared += payoff * payoff;
}


//  GENERATE MANY PATHS

void BlackScholesMCPricer::generate(int nb_paths)
{
    if (nb_paths <= 0) return;

    for (int i = 0; i < nb_paths; ++i)
    {
        simulateOnePath();
    }
}


//  PRICE (MEAN DISCOUNTED PAYOFF)

double BlackScholesMCPricer::operator()()
{
    if (_nbPaths == 0 || _option == nullptr)
        return 0.0;

    double n = static_cast<double>(_nbPaths);

    double meanPayoff = _sumPayoff / n;

    double T = _option->getExpiry();
    double discount = std::exp(-_r * T);

    double price = discount * meanPayoff;
    return price;
}


//  CONFIDENCE INTERVAL 95%

std::vector<double> BlackScholesMCPricer::confidenceInterval()
{
    std::vector<double> ci(2, 0.0);

    if (_nbPaths <= 1 || _option == nullptr)
    {
        double p = (*this)();
        ci[0] = p;
        ci[1] = p;
        return ci;
    }

    double n = static_cast<double>(_nbPaths);

    double meanPayoff  = _sumPayoff / n;
    double meanPayoff2 = _sumPayoffSquared / n;

    double varPayoff = meanPayoff2 - meanPayoff * meanPayoff;
    if (varPayoff < 0.0) varPayoff = 0.0;

    double stdPayoff = std::sqrt(varPayoff);

    double T = _option->getExpiry();
    double discount = std::exp(-_r * T);

    double price = discount * meanPayoff;
    double stdPrice = discount * stdPayoff / std::sqrt(n);

    double alpha = 1.96; // 95%
    ci[0] = price - alpha * stdPrice;
    ci[1] = price + alpha * stdPrice;

    return ci;
}

//  NUMBER OF PATHS

int BlackScholesMCPricer::getNbPaths() const
{
    return _nbPaths;
}
