#include "CRRPricer.h"
#include <cmath>
#include <stdexcept>
using namespace std;

CRRPricer::CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate): _option(option), _depth(depth), _S0(asset_price), _U(up), _D(down), _R(interest_rate), _computed(false) 
{
    // check conditions
    if (!_option) { throw invalid_argument("null option"); }
    if (_depth < 0) { throw invalid_argument("depth must be non-negative");}
    if (_option->isAsianOption()) { throw invalid_argument("asian not supported"); }
    if (!(_D < _R && _R < _U)) { throw invalid_argument("no arbitrage");}

    _priceTree.setDepth(_depth);
    _exerciseTree.setDepth(_depth);
}

CRRPricer::CRRPricer(Option* option, int depth, double asset_price, double r, double volatility): _option(option), _depth(depth), _S0(asset_price), _computed(false) 
{
    // check conditions
    if (!_option) { throw invalid_argument("null option"); }
    if (_depth <= 0) { throw invalid_argument("depth must be positive");}
    if (_option->isAsianOption()) { throw invalid_argument("asian not supported");}
    
    // Black-Scholes approximation parameters
    double T = _option->getExpiry();
    double h = T / _depth;
    _U = exp((r + 0.5 * volatility * volatility) * h + volatility * sqrt(h)) - 1.0;
    _D = exp((r + 0.5 * volatility * volatility) * h - volatility * sqrt(h)) - 1.0;
    _R = exp(r * h) - 1.0;
    if (!(_D < _R && _R < _U)) { throw invalid_argument("no arbitrage");}


    _priceTree.setDepth(_depth);
    _exerciseTree.setDepth(_depth);
}

void CRRPricer::compute() {
    double q = (_R - _D) / (_U - _D);
    double disc = 1.0 / (1.0 + _R);
    bool american = _option->isAmericanOption();

    // terminal payoffs
    for (int i = 0; i <= _depth; i++) {
        double S = _S0 * pow(1.0 + _U, i) * pow(1.0 + _D, _depth - i);
        _priceTree.setNode(_depth, i, _option->payoff(S));
        _exerciseTree.setNode(_depth, i, false);
    }

    // backward induction
    for (int n = _depth - 1; n >= 0; n--) {
        for (int i = 0; i <= n; i++) {
            double cont = (q * _priceTree.getNode(n + 1, i + 1) + (1.0 - q) * _priceTree.getNode(n + 1, i)) / (1.0 + _R);
            double value = cont;
            bool exercise = false;

            // american case
            if (_option->isAmericanOption()) {
                double S = _S0 * pow(1.0 + _U, i) * pow(1.0 + _D, n - i);
                double intrinsic = _option->payoff(S);

                if (intrinsic >= cont) {
                    value = intrinsic;
                    exercise = true;
                }
            }

            _priceTree.setNode(n, i, value);
            _exerciseTree.setNode(n, i, exercise);
        }
    }

    _computed = true;
}

double CRRPricer::get(int n, int i) { return _priceTree.getNode(n, i);}

bool CRRPricer::getExercise(int n, int i) { return _exerciseTree.getNode(n, i); }

double CRRPricer::operator()(bool closed_form) {
    if (closed_form) {
        // closed form = european only
        if (_option->isAmericanOption()) { throw invalid_argument("closed form only for european");}

        double q = (_R - _D) / (_U - _D);
        if (q <= 0.0 || q >= 1.0) { throw invalid_argument("invalid q"); }

        double disc = pow(1.0 + _R, _depth);
        double sum = 0.0;
        double comb = 1.0;
        double prob = pow(1.0 - q, _depth);

        for (int i = 0; i <= _depth; i++) {
            double S = _S0 * pow(1.0 + _U, i) * pow(1.0 + _D, _depth - i);
            double pay = _option->payoff(S);
            sum += comb * prob * pay;

            // update to next i
            if (i < _depth) {
                comb = comb * (_depth - i) / (i + 1.0);
                prob = prob * (q / (1.0 - q));
            }
        }

        return sum / disc;
    }

    if (!_computed) { compute();}
    return _priceTree.getNode(0, 0);
}