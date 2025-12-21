#include "CRRPricer.h"
#include <cmath>
#include <stdexcept>

using namespace std;

CRRPricer::CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate) : _option(option), _depth(depth), _S0(asset_price), _U(up), _D(down), _R(interest_rate), _computed(false) {

    if (!_option) {
        throw invalid_argument("option pointer is null");
    }
    if (depth < 0) {
        throw invalid_argument("depth must be non-negative");
    }

    // cannot price Asian options in CRR
    if (_option->isAsianOption()) {
        throw invalid_argument("CRRPricer cannot handle Asian options");
    }

    if (!(_D < _R && _R < _U)) {
        throw invalid_argument("no-arbitrage condition not satisfied");
    }

    _priceTree.setDepth(_depth);
    _exerciseTree.setDepth(_depth);
}

CRRPricer::CRRPricer(Option* option, int depth, double asset_price, double r, double volatility) : _option(option), _depth(depth), _S0(asset_price), _computed(false) {

    if (!_option) {
        throw invalid_argument("option pointer is null");
    }
    if (depth <= 0) {
        throw invalid_argument("depth must be positive");
    }

    if (_option->isAsianOption()) {
        throw invalid_argument("CRRPricer cannot handle Asian options");
    }

    double T = _option->getExpiry();
    double h = T / static_cast<double>(_depth);

    // CRR parameters from Black-Scholes limit
    double u = exp(volatility * sqrt(h));
    double d = 1.0 / u;

    _U = u - 1.0;
    _D = d - 1.0;
    _R = exp(r * h) - 1.0;
    if (!(_D < _R && _R < _U)) {
        throw invalid_argument("no-arbitrage condition not satisfied");
    }
    
    if (!(_D < _R && _R < _U)) {
        throw invalid_argument("no-arbitrage condition not satisfied");
    }

    _priceTree.setDepth(_depth);
    _exerciseTree.setDepth(_depth);
}

void CRRPricer::buildStockTree(BinaryTree<double>& stockTree) const { stockTree.setDepth(_depth);

    for (int n = 0; n <= _depth; ++n) {
        for (int i = 0; i <= n; ++i) {
            double S = _S0* pow(1.0 + _U, i)  * pow(1.0 + _D, n - i);
            stockTree.setNode(n, i, S);
        }
    }
}

void CRRPricer::compute() {
    BinaryTree<double> stockTree;
    buildStockTree(stockTree);

    double q = (_R - _D) / (_U - _D);
    double disc = 1.0 / (1.0 + _R);
    bool american = _option->isAmericanOption();

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
            double cont = (q * _priceTree.getNode(n + 1, i + 1) + (1.0 - q) * _priceTree.getNode(n + 1, i)) / (1.0 + _R);

            double nodeVal = cont;
            bool exercise = false;

            if (american) {
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
        if (_option->isAmericanOption()) {
            throw invalid_argument("closed_form not available for American options");
        }
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
                comb *= static_cast<double>(_depth - k + 1)/ static_cast<double>(k);
            }

            sum += comb* pow(q, i) * pow(1.0 - q, _depth - i)* h;
        }

        double disc = pow(1.0 + _R, _depth);
        return sum / disc;
    }

    if (!_computed) {
        compute();
    }
    return _priceTree.getNode(0, 0);
}
