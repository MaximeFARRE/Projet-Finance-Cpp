#include "CRRPricer.h"
#include <cmath>
#include <stdexcept>
#include <algorithm>

// Prix de l'actif au noeud (n,i) : S(n,i) = S0 (1+U)^i (1+D)^(n-i)
double CRRPricer::stockPrice(int n, int i) const {
    double upFactor = std::pow(1.0 + _U, i);
    double downFactor = std::pow(1.0 + _D, n - i);
    return _S0 * upFactor * downFactor;
}

CRRPricer::CRRPricer(Option* option,
    int depth,
    double asset_price,
    double up,
    double down,
    double interest_rate)
    : _option(option),
    _depth(depth),
    _S0(asset_price),
    _U(up),
    _D(down),
    _R(interest_rate),
    _tree(),
    _computed(false)
{
    // Vérification pointeur
    if (_option == nullptr) {
        throw std::invalid_argument("Option pointer cannot be null");
    }

    // CRRPricer ne doit PAS traiter les options asiatiques
    if (_option->isAsianOption()) {
        throw std::invalid_argument("CRRPricer cannot price Asian options");
    }

    // Profondeur de l'arbre
    if (_depth < 0) {
        throw std::invalid_argument("Depth must be non-negative");
    }

    // Condition de non-arbitrage : D < R < U
    if (!(_D < _R && _R < _U)) {
        throw std::invalid_argument("Arbitrage detected: need D < R < U");
    }

    // Initialisation de l'arbre de prix H(n,i)
    _tree.setDepth(_depth);
}


// Implémente la procédure CRR par backward induction
void CRRPricer::compute() {
    // Probabilité risque-neutre
    double q = (_R - _D) / (_U - _D);

    int N = _depth;

    // 1) À maturité : H(N,i) = h(S(N,i))
    for (int i = 0; i <= N; ++i) {
        double SNi = stockPrice(N, i);
        double payoffNi = _option->payoff(SNi);
        _tree.setNode(N, i, payoffNi);
    }

    // 2) Backward induction pour n = N-1 ... 0
    for (int n = N - 1; n >= 0; --n) {
        for (int i = 0; i <= n; ++i) {
            double H_up = _tree.getNode(n + 1, i + 1);
            double H_down = _tree.getNode(n + 1, i);
            double Hni = (q * H_up + (1.0 - q) * H_down) / (1.0 + _R);
            _tree.setNode(n, i, Hni);
        }
    }

    _computed = true;
}

// Getter : renvoie H(n,i)
double CRRPricer::get(int n, int i) const {
    return _tree.getNode(n, i);
}

// Calcul de C(N,i) = "N choose i" en double, de manière simple
static double binomialCoeff(int N, int i) {
    if (i < 0 || i > N) return 0.0;
    if (i == 0 || i == N) return 1.0;

    // on utilise une forme multiplicative pour éviter les gros factorielles
    double result = 1.0;
    int k = std::min(i, N - i); // C(N,i) = C(N, N-i)
    for (int j = 1; j <= k; ++j) {
        result *= (N - k + j);
        result /= j;
    }
    return result;
}

// Prix de l'option
double CRRPricer::operator()(bool closed_form) {
    int N = _depth;

    // Probabilité risque-neutre
    double q = (_R - _D) / (_U - _D);

    if (!closed_form) {
        // Utilise la procédure CRR classique
        if (!_computed) {
            compute();
        }
        // H(0,0)
        return _tree.getNode(0, 0);
    }
    else {
        // Utilise la formule fermée :
        // H(0,0) = (1/(1+R)^N) * sum_{i=0..N} C(N,i) q^i (1-q)^(N-i) h(S(N,i))
        double discount = std::pow(1.0 + _R, N);
        double sum = 0.0;

        for (int i = 0; i <= N; ++i) {
            double comb = binomialCoeff(N, i);
            double prob = std::pow(q, i) * std::pow(1.0 - q, N - i);
            double SNi = stockPrice(N, i);
            double payoffNi = _option->payoff(SNi);

            sum += comb * prob * payoffNi;
        }

        return sum / discount;
    }
}
