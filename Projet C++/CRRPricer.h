#pragma once
#include "Option.h"
#include "BinaryTree.h"

class CRRPricer {
private:
    Option* _option;
    int _depth;
    double _S0, _U, _D, _R;
    bool _computed;

    BinaryTree<double> _priceTree;
    BinaryTree<bool> _exerciseTree;

public:
    CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate);
    // Black-Scholes approximation
    CRRPricer(Option* option, int depth, double asset_price, double r, double volatility);

    void compute();
    double get(int n, int i);
    bool getExercise(int n, int i);

    double operator()(bool closed_form = false);
};