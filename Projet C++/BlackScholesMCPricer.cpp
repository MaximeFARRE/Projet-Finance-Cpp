#include "BlackScholesMCPricer.h"
#include "AsianOption.h"
#include "MT.h"

#include <cmath>
#include <stdexcept>

// Constructeur
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
    _sumPayoffSquared(0.0),
    _timeSteps()
{
    if (_option == nullptr) {
        throw std::invalid_argument("Option pointer cannot be null");
    }

    // Si l'option est asiatique, on récupère ses timeSteps
    if (_option->isAsianOption()) {
        AsianOption* asian = dynamic_cast<AsianOption*>(_option);
        if (asian == nullptr) {
            throw std::runtime_error("isAsianOption() = true but dynamic_cast<AsianOption*> failed");
        }
        _timeSteps = asian->getTimeSteps();
    }
    else {
        // Sinon, on simule avec un seul point à l'échéance T
        double T = _option->getExpiry();
        _timeSteps.clear();
        _timeSteps.push_back(T);
    }
}

// Simule UNE trajectoire et met à jour les sommes
void BlackScholesMCPricer::simulateOnePath()
{
    if (_timeSteps.empty()) {
        throw std::runtime_error("timeSteps is empty in simulateOnePath");
    }

    std::vector<double> path;
    path.reserve(_timeSteps.size());

    double S = _S0;
    double prev_t = 0.0;

    MT& mt = MT::getInstance();

    for (std::size_t k = 0; k < _timeSteps.size(); ++k) {
        double t = _timeSteps[k];
        double dt = t - prev_t;

        if (dt < 0.0) {
            throw std::runtime_error("timeSteps must be non-decreasing");
        }

        if (dt > 0.0) {
            double Z = mt.rand_norm();  // N(0,1)

            double drift = (_r - 0.5 * _sigma * _sigma) * dt;
            double diff_term = _sigma * std::sqrt(dt) * Z;

            S = S * std::exp(drift + diff_term);
        }

        path.push_back(S);
        prev_t = t;
    }

    double payoff = _option->payoffPath(path);

    _nbPaths++;
    _sumPayoff += payoff;
    _sumPayoffSquared += payoff * payoff;
}

// Génère nbPaths trajectoires supplémentaires
void BlackScholesMCPricer::generate(long nbPaths)
{
    if (nbPaths <= 0) return;

    for (long i = 0; i < nbPaths; ++i) {
        simulateOnePath();
    }
}

// Prix estimé courant
double BlackScholesMCPricer::operator()() const
{
    if (_nbPaths == 0) {
        throw std::runtime_error("No paths generated yet");
    }

    double meanPayoff = _sumPayoff / static_cast<double>(_nbPaths);
    double T = _option->getExpiry();
    double discount = std::exp(-_r * T);

    return discount * meanPayoff;
}

// Intervalle de confiance 95% : [low, high]
std::vector<double> BlackScholesMCPricer::confidenceInterval() const
{
    if (_nbPaths == 0) {
        throw std::runtime_error("No paths generated yet");
    }

    double n = static_cast<double>(_nbPaths);
    double meanPayoff = _sumPayoff / n;
    double meanSquare = _sumPayoffSquared / n;

    double varPayoff = meanSquare - meanPayoff * meanPayoff;
    if (varPayoff < 0.0) varPayoff = 0.0;  // sécurité numérique

    double stdPayoff = std::sqrt(varPayoff);
    double T = _option->getExpiry();
    double discount = std::exp(-_r * T);

    double price = discount * meanPayoff;
    double halfWidth = 1.96 * discount * stdPayoff / std::sqrt(n);

    std::vector<double> ci(2);
    ci[0] = price - halfWidth;
    ci[1] = price + halfWidth;
    return ci;
}

long BlackScholesMCPricer::getNbPaths() const
{
    return _nbPaths;
}
