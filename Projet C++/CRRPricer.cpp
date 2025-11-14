#include "CRRPricer.h"
#include "AmericanOption.h"
#include <cmath>
#include <stdexcept>
#include <algorithm>

// Prix de l'actif au noeud (n,i) : S(n,i) = S0 (1+U)^i (1+D)^(n-i)
double CRRPricer::stockPrice(int n, int i) const {
    double upFactor = std::pow(1.0 + _U, i);
    double downFactor = std::pow(1.0 + _D, n - i);
    return _S0 * upFactor * downFactor;
}

// Constructeur principal : on donne U, D, R
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
    _exercise(),
    _computed(false)
{
    if (_option == nullptr) {
        throw std::invalid_argument("Option pointer cannot be null");
    }

    // CRR ne traite pas les Asiatiques
    if (_option->isAsianOption()) {
        throw std::invalid_argument("CRRPricer cannot price Asian options");
    }

    if (_depth < 0) {
        throw std::invalid_argument("Depth must be non-negative");
    }

    // Condition de non-arbitrage : D < R < U
    if (!(_D < _R && _R < _U)) {
        throw std::invalid_argument("Arbitrage detected: need D < R < U");
    }

    _tree.setDepth(_depth);
    _exercise.setDepth(_depth);
}

// Constructeur Black-Scholes approx : on donne r (continu) et sigma
CRRPricer::CRRPricer(Option* option,
    int depth,
    double asset_price,
    double r,
    double volatility)
    : _option(option),
    _depth(depth),
    _S0(asset_price),
    _U(0.0),
    _D(0.0),
    _R(0.0),
    _tree(),
    _exercise(),
    _computed(false)
{
    if (_option == nullptr) {
        throw std::invalid_argument("Option pointer cannot be null");
    }

    if (_option->isAsianOption()) {
        throw std::invalid_argument("CRRPricer cannot price Asian options");
    }

    if (_depth <= 0) {
        throw std::invalid_argument("Depth must be positive");
    }

    double T = _option->getExpiry();
    double h = T / static_cast<double>(_depth);

    // Paramètres binomiaux pour approx Black-Scholes (section 8 du sujet)
    double sigma2 = volatility * volatility;

    _U = std::exp((r + 0.5 * sigma2) * h + volatility * std::sqrt(h)) - 1.0;
    _D = std::exp((r + 0.5 * sigma2) * h - volatility * std::sqrt(h)) - 1.0;
    _R = std::exp(r * h) - 1.0;

    // Vérification arbitrage
    if (!(_D < _R && _R < _U)) {
        throw std::invalid_argument("Arbitrage detected in BS approximation: need D < R < U");
    }

    _tree.setDepth(_depth);
    _exercise.setDepth(_depth);
}

// Implémente la procédure CRR
void CRRPricer::compute() {
    double q = (_R - _D) / (_U - _D);
    int N = _depth;

    bool isAmerican = _option->isAmericanOption();

    // 1) À maturité : même valeur que l'Européenne
    for (int i = 0; i <= N; ++i) {
        double SNi = stockPrice(N, i);
        double payoffNi = _option->payoff(SNi);
        _tree.setNode(N, i, payoffNi);

        if (isAmerican) {
            // à l'échéance, on exerce si payoff > 0 (intrinsic > 0)
            _exercise.setNode(N, i, payoffNi > 0.0);
        }
        else {
            _exercise.setNode(N, i, false);
        }
    }

    // 2) Backward induction
    for (int n = N - 1; n >= 0; --n) {
        for (int i = 0; i <= n; ++i) {
            double H_up = _tree.getNode(n + 1, i + 1);
            double H_down = _tree.getNode(n + 1, i);

            double continuation = (q * H_up + (1.0 - q) * H_down) / (1.0 + _R);

            if (!isAmerican) {
                // Européenne : comme avant
                _tree.setNode(n, i, continuation);
                _exercise.setNode(n, i, false);
            }
            else {
                // Américaine : max(continuation, intrinsic)
                double Sni = stockPrice(n, i);
                double intrinsic = _option->payoff(Sni);

                double Hni = std::max(continuation, intrinsic);
                _tree.setNode(n, i, Hni);

                bool exerciseHere = (intrinsic >= continuation);
                _exercise.setNode(n, i, exerciseHere);
            }
        }
    }

    _computed = true;
}

// Getter : H(n,i)
double CRRPricer::get(int n, int i) const {
    return _tree.getNode(n, i);
}

// Getter : politique d'exercice
bool CRRPricer::getExercise(int n, int i) const {
    return _exercise.getNode(n, i);
}

// Calcul de C(N,i) = "N choose i"
static double binomialCoeff(int N, int i) {
    if (i < 0 || i > N) return 0.0;
    if (i == 0 || i == N) return 1.0;

    double result = 1.0;
    int k = std::min(i, N - i);
    for (int j = 1; j <= k; ++j) {
        result *= (N - k + j);
        result /= j;
    }
    return result;
}

// Prix de l'option
double CRRPricer::operator()(bool closed_form) {
    int N = _depth;
    double q = (_R - _D) / (_U - _D);

    bool isAmerican = _option->isAmericanOption();

    if (isAmerican) {
        // Pas de closed-form pour les Américaines
        if (!_computed) {
            compute();
        }
        return _tree.getNode(0, 0);
    }

    // Européenne
    if (!closed_form) {
        if (!_computed) {
            compute();
        }
        return _tree.getNode(0, 0);
    }
    else {
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
