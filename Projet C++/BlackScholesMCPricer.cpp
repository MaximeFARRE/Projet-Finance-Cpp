#include "BlackScholesMCPricer.h"
#include "AsianOption.h"
#include "MT.h"
#include <cmath>
#include <iostream>
using namespace std;

//  CONSTRUCTEUR

BlackScholesMCPricer::BlackScholesMCPricer(Option* option,
    double S0,
    double r,
    double sigma)
    : _option(option),
    _S0(S0),
    _r(r),
    _sigma(sigma),
    _nbPaths(0),
    _sumPayoff(0.0),
    _sumPayoffSquared(0.0)
{
    if (_option == nullptr)
    {
        // cas d'erreur simple
        std::cout << "erreur option = nullptr dans BlackScholesMCPricer" << std::endl;
        return;
    }

    // si c'est une option asiatique
    if (_option->isAsianOption())
    {
        AsianOption* asian = dynamic_cast<AsianOption*>(_option);

        //  recupere dates de fixing
        _timeSteps = asian->getTimeSteps();
    }
    else
    {
        // option europeenne : un seul time step a l'echeance
        double T = _option->getExpiry();
        _timeSteps.clear();
        _timeSteps.push_back(T);
    }
}

// -- simule UNE trajectoire --
void BlackScholesMCPricer::simulateOnePath()
{
    if (_option == nullptr) return;

    // generateur aleatoire global du projet
    MT& mt = MT::getInstance();

    double S = _S0;      // prix au debut
    double prev_t = 0.0; // temps precedent

    std::vector<double> path;  // on stocke la trajectoire
    double t,dt;
    double Z;

    // on parcourt tous les instants de temps
    for (std::size_t i = 0; i < _timeSteps.size(); ++i)
    {
        t = _timeSteps[i];
        dt = t - prev_t;  

        if (dt > 0.0)
        {
            // tirage aleatoire normal
            Z = mt.rand_norm();

            // drift et diffusion du modele Black-Scholes
            double drift = (_r - 0.5 * _sigma * _sigma) * dt;
            double diffusion = _sigma * std::sqrt(dt) * Z;

            // formule pour simuler S_t log-normal
            S = S * std::exp(drift + diffusion);
        }

        // on ajoute la valeur courante a la trajectoire
        path.push_back(S);
        prev_t = t;
    }

    // calcul du payoff sur la trajectoire
    double payoff = _option->payoffPath(path);

    // mise a jour des statistiques pour la moyenne et la variance
    _nbPaths += 1;
    _sumPayoff += payoff;
    _sumPayoffSquared += payoff * payoff;
}

//  GENERATE nbPaths 
void BlackScholesMCPricer::generate(long nbPaths)
{
    if (nbPaths <= 0) return;

    // on appelle simulateOnePath nbPaths fois
    for (long i = 0; i < nbPaths; ++i)
    {
        simulateOnePath();
    }
}

//  --Pricer Monte Carlo --

double BlackScholesMCPricer::operator()() const
{
    if (_nbPaths == 0 || _option == nullptr)
        return 0.0;

    // moyenne des payoffs
    double meanPayoff = _sumPayoff / static_cast<double>(_nbPaths);

    // actualisation 
    double T = _option->getExpiry();
    double discount = std::exp(-_r * T);

    double price = discount * meanPayoff;
    return price;
}
 
//  inter de confiance 95%

std::vector<double> BlackScholesMCPricer::confidenceInterval() const
{
    std::vector<double> ci(2, 0.0);

    if (_nbPaths <= 1 || _option == nullptr)
    {
        // pas assez de donnees : on met juste le prix actuel
        double p = (*this)();
        ci[0] = p;
        ci[1] = p;
        return ci;
    }

    double n = static_cast<double>(_nbPaths);

    // E[X] et E[X^2]
    double meanPayoff = _sumPayoff / n;
    double meanPayoff2 = _sumPayoffSquared / n;

    // variance des payoffs
    double varPayoff = meanPayoff2 - meanPayoff * meanPayoff;
    if (varPayoff < 0.0) varPayoff = 0.0; // securite numerique

    double stdPayoff = std::sqrt(varPayoff); // ecart-type des payoffs

    // payoff au prix en actualisant
    double T = _option->getExpiry();
    double discount = std::exp(-_r * T);

    double price = discount * meanPayoff;

    // ecart-type de la moyenne
    double stdPrice = discount * stdPayoff / std::sqrt(n);

    // 1.96 ~ quantile pour 95% de confiance
    double alpha = 1.96;
    ci[0] = price - alpha * stdPrice;
    ci[1] = price + alpha * stdPrice;

    return ci;
}

// =============================
//  NOMBRE DE TRAJECTOIRES
// =============================
long BlackScholesMCPricer::getNbPaths() const
{
    return _nbPaths;
}
