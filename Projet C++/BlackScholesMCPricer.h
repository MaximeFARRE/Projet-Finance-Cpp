#pragma once

#include "Option.h"
#include <vector>

// pricer Monte Carlo Black-Scholes
class BlackScholesMCPricer
{
private:
    Option* _option;              // pointeur vers l'option
    double _S0;                   // prix initial
    double _r;                    // taux sans risque
    double _sigma;                // volatilite

    long _nbPaths;                // nombre total de trajectoires simulees
    double _sumPayoff;            // somme des payoffs
    double _sumPayoffSquared;     // somme des payoffs^2 (pour la variance)

    std::vector<double> _timeSteps;   // dates de simulation (fixings ou juste T)

    // simule une trajectoire 
    void simulateOnePath();

public:
    // constructeur
    BlackScholesMCPricer(Option* option, double S0, double r, double sigma);

    // genere nbPaths trajectoires
    void generate(long nbPaths);

    // renvoie le prix estime (moyenne des payoffs)
    double operator()() const;

    // intervalle de confiance a 95%
    std::vector<double> confidenceInterval() const;

    // renvoie le nombre de trajectoires simulees
    long getNbPaths() const;
};
