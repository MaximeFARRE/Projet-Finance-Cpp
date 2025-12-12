#pragma once

#include "Option.h"
#include <vector>

// Monte Carlo pricer under Black-Scholes model
class BlackScholesMCPricer
{
private:
    Option* _option;              // pointer to option
    double _S0;                   // initial price
    double _r;                    // interest rate
    double _sigma;                // volatility

    int _nbPaths;                 // number of simulated paths
    double _sumPayoff;            // sum of payoffs
    double _sumPayoffSquared;     // sum of payoffs^2 (for variance)

    std::vector<double> _timeSteps;   // times of simulation

    // simulate one path and update statistics
    void simulateOnePath();

public:
    // constructor
    BlackScholesMCPricer(Option* option,
                         double initial_price,
                         double interest_rate,
                         double volatility);

    // generate nb_paths trajectories
    void generate(int nb_paths);

    // estimated price (mean of discounted payoffs)
    double operator()();

    // confidence interval 95%
    std::vector<double> confidenceInterval();

    // number of simulated paths
    int getNbPaths() const;
};
//