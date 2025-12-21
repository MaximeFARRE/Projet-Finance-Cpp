#pragma once

#include "Option.h"
#include <vector>
using namespace std;

// Monte Carlo pricer for Black-Scholes model
class BlackScholesMCPricer 
{
private:
    Option* _option;              
    double _S0;                  
    double _r;                    
    double _sigma;                

    long _nbPaths;                
    double _sumPayoff;            
    double _sumPayoffSquared;     

    vector<double> _timeSteps;  

public:
    // Constructor
    BlackScholesMCPricer(Option* option, double S0, double r, double sigma);

    // Simulate nbPaths 
    void generate(long nbPaths);

    // Return the estimated option price
    double operator()() const;

    // Return a 95% confidence interval
    vector<double> confidenceInterval() const;

    // Return the number of simulated paths
    long getNbPaths() const;
};
//
