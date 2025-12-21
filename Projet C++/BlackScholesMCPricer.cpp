#include "BlackScholesMCPricer.h"
#include "AsianOption.h"
#include "MT.h"

#include <cmath>
#include <iostream>
#include <limits>

using namespace std;

//constructor
BlackScholesMCPricer::BlackScholesMCPricer(Option* option,double S0, double r, double sigma) : _option(option), _S0(S0), _r(r), _sigma(sigma),  _nbPaths(0), _sumPayoff(0.0), _sumPayoffSquared(0.0)
{
    if (_option == nullptr)
    {
        //  error case
        cerr << "Error option is nullptr" << endl;
        return;
    }

    // If is an Asian option use fixing times
    if (_option->isAsianOption())
    {
        AsianOption* asian = dynamic_cast<AsianOption*>(_option);
        if (asian != nullptr)
        {
            _timeSteps = asian->getTimeSteps();
        }
    }
    else
    {
        // European option
        double T = _option->getExpiry();
        _timeSteps.clear();
        _timeSteps.push_back(T); //we keep the push_back for because it's only executed once here
    }
}


//path generation
void BlackScholesMCPricer::generate(long nbPaths)
{
    if (nbPaths <= 0)
        return;

    if (_option == nullptr)
        return;

    const size_t nSteps = _timeSteps.size();
    if (nSteps == 0)
        return;

    MT& mt = MT::getInstance();

    vector<double> path(nSteps);

    double S;
    double prev_t;
    double t;
    double dt;
    double Z;

    // Simulate nbPaths trajectories
    for (long pathIndex = 0; pathIndex < nbPaths; ++pathIndex)
    {
        S = _S0;       
        prev_t = 0.0;  

        for (size_t i = 0; i < nSteps; ++i)
        {
            t = _timeSteps[i];
            dt = t - prev_t;

            if (dt > 0.0)
            {
                // standard normal random variable
                Z = mt.rand_norm();

                // Drift and diffusion in Black-Scholes model
                double drift = (_r - 0.5 * _sigma * _sigma) * dt;
                double diffusion = _sigma * sqrt(dt) * Z;

                S = S * exp(drift + diffusion);

                if (!isfinite(S) || S < 0.0) {
                    S = 0.0;
                }
            }

            // Store price 
            path[i] = S;
            prev_t = t;
        }

        // Compute payoff for this path
        double payoff = _option->payoffPath(path);

        _nbPaths += 1;
        _sumPayoff += payoff;
        _sumPayoffSquared += payoff * payoff;
    }
}


//MC pricer
double BlackScholesMCPricer::operator()() const
{
    if (_nbPaths == 0 || _option == nullptr)
        return 0.0;

    // Mean payoff
    double meanPayoff = _sumPayoff / _nbPaths;

    // Discount to present value
    double T = _option->getExpiry();
    double discount = exp(-_r * T);

    double price = discount * meanPayoff;
    return price;
}


//95% confidence interval
vector<double> BlackScholesMCPricer::confidenceInterval() const
{
    vector<double> ci(2, 0.0);
    
    if (_nbPaths < 1000 || _option == nullptr) {
        ci[0] = -1e100;
        ci[1] =  1e100;
        return ci;
    }

    double n = _nbPaths;

    // means
    double meanPayoff = _sumPayoff / n;
    double meanPayoff2 = _sumPayoffSquared / n;

    // Var
    double varPayoff = meanPayoff2 - meanPayoff * meanPayoff;
    if (varPayoff < 0.0)
        varPayoff = 0.0;  //  safety

    double stdPayoff = sqrt(varPayoff);  // standard deviation of payoffs

    // Convert payoff statistics to price statistics 
    double T = _option->getExpiry();
    double discount = exp(-_r * T);

    double price = discount * meanPayoff;

    // Standard deviation of the mean price
    double stdPrice = discount * stdPayoff / sqrt(n);

    // 1.96 -> 95% CI for the mean
    double alpha = 1.96;
    ci[0] = price - alpha * stdPrice;
    ci[1] = price + alpha * stdPrice;

    return ci;
}

//number of simulated paths
long BlackScholesMCPricer::getNbPaths() const
{
    return _nbPaths;
}