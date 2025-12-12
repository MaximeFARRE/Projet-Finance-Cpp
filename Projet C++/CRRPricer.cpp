#include "CRRPricer.h"
#include "AsianOption.h"
#include <cmath>
#include <stdexcept>

CRRPricer::CRRPricer(Option* option, int depth,
                     double asset_price,
                     double up, double down,
                     double interest_rate)
    : _option(option),
      _depth(depth),
      _S0(asset_price),
      _U(up),
      _D(down),
      _R(interest_rate),
      _computed(false) {

    if (!_option) {
        throw std::invalid_argument("option pointer is null");
    }
    if (depth < 0) {
        throw std::invalid_argument("depth must be non-negative");
    }

    // cannot price Asian options in CRR
    if (_option->isAsianOption()) {
        throw std::invalid_argument("CRRPricer cannot handle Asian options");
    }

    if (!(_D < _R && _R < _U)) {
        throw std::invalid_argument("no-arbitrage condition not satisfied");
    }

    _priceTree.setDepth(_depth);
    _exerciseTree.setDepth(_depth);
}

CRRPricer::CRRPricer(Option* option, int depth,
                     double asset_price,
                     double r, double volatility)
    : _option(option),
      _depth(depth),
      _S0(asset_price),
      _computed(false) {

    if (!_option) {
        throw std::invalid_argument("option pointer is null");
    }
    if (depth <= 0) {
        throw std::invalid_argument("depth must be positive");
    }

    if (_option->isAsianOption()) {
        throw std::invalid_argument("CRRPricer cannot handle Asian options");
    }

    double T = _option->getExpiry();
    double h = T / static_cast<double>(_depth);

    _U = std::exp((r + 0.5 * volatility * volatility) * h
                  + volatility * std::sqrt(h)) - 1.0;
    _D = std::exp((r + 0.5 * volatility * volatility) * h
                  - volatility * std::sqrt(h)) - 1.0;
    _R = std::exp(r * h) - 1.0;

    if (!(_D < _R && _R < _U)) {
        throw std::invalid_argument("no-arbitrage condition not satisfied");
    }

    _priceTree.setDepth(_depth);
    _exerciseTree.setDepth(_depth);
}
//
void CRRPricer::buildStockTree(BinaryTree<double>& stockTree) const {
    stockTree.setDepth(_depth);

    for (int n = 0; n <= _depth; ++n) {
        for (int i = 0; i <= n; ++i) {
            double S = _S0
                * std::pow(1.0 + _U, i)
                * std::pow(1.0 + _D, n - i);
            stockTree.setNode(n, i, S);
        }
    }
}

void CRRPricer::compute() {
    BinaryTree<double> stockTree;
    buildStockTree(stockTree);

    double q = (_R - _D) / (_U - _D);

    // terminal values
    for (int i = 0; i <= _depth; ++i) {
        double S = stockTree.getNode(_depth, i);
        double value = _option->payoff(S);
        _priceTree.setNode(_depth, i, value);
        _exerciseTree.setNode(_depth, i, false);
    }

    // backward induction
    for (int n = _depth - 1; n >= 0; --n) {
        for (int i = 0; i <= n; ++i) {
            double cont = (q * _priceTree.getNode(n + 1, i + 1)
                          + (1.0 - q) * _priceTree.getNode(n + 1, i))
                          / (1.0 + _R);

            double nodeVal = cont;
            bool exercise = false;

            if (_option->isAmericanOption()) {
                double S = stockTree.getNode(n, i);
                double intrinsic = _option->payoff(S);
                if (intrinsic >= cont) {
                    nodeVal = intrinsic;
                    exercise = true;
                }
            }

            _priceTree.setNode(n, i, nodeVal);
            _exerciseTree.setNode(n, i, exercise);
        }
    }

    _computed = true;
}

double CRRPricer::get(int n, int i) {
    return _priceTree.getNode(n, i);
}

bool CRRPricer::getExercise(int n, int i) {
    return _exerciseTree.getNode(n, i);
}

double CRRPricer::operator()(bool closed_form) {
    if (closed_form) {
        BinaryTree<double> stockTree;
        buildStockTree(stockTree);

        double q = (_R - _D) / (_U - _D);
        double sum = 0.0;

        for (int i = 0; i <= _depth; ++i) {
            double S = stockTree.getNode(_depth, i);
            double h = _option->payoff(S);

            // simple binomial coefficient C(N, i)
            double comb = 1.0;
            for (int k = 1; k <= i; ++k) {
                comb *= static_cast<double>(_depth - k + 1)
                      / static_cast<double>(k);
            }

            sum += comb
                * std::pow(q, i)
                * std::pow(1.0 - q, _depth - i)
                * h;
        }

        double disc = std::pow(1.0 + _R, _depth);
        return sum / disc;
    }

    if (!_computed) {
        compute();
    }
    return _priceTree.getNode(0, 0);
}
