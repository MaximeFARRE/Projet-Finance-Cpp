#pragma once

#include "Option.h"
#include <vector>

// Monte Carlo pricer sous Black-Scholes
class BlackScholesMCPricer {
private:
    Option* _option;              // pointeur vers l'option
    double _S0;                   // prix initial
    double _r;                    // taux sans risque
    double _sigma;                // volatilité

    long _nbPaths;                // nombre total de trajectoires générées
    double _sumPayoff;            // somme des payoffs
    double _sumPayoffSquared;     // somme des payoffs^2 (pour la variance)

    std::vector<double> _timeSteps; // instants de simulation (t1,...,tm)

    // Simule UNE trajectoire et met à jour les sommes
    void simulateOnePath();

public:
    // Constructeur
    BlackScholesMCPricer(Option* option,
        double S0,
        double r,
        double sigma);

    // Génère nbPaths trajectoires supplémentaires
    void generate(long nbPaths);

    // Renvoie le prix estimé actuel
    double operator()() const;

    // Intervalle de confiance 95% : [low, high]
    std::vector<double> confidenceInterval() const;

    // Retourne le nombre total de trajectoires simulées
    long getNbPaths() const;

};
