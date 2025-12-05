#pragma once
#include "Option.h"
#include "BinaryTree.h"

class CRRPricer {
private:
    Option* _option;
    int _depth;
    double _S0;
    double _U;
    double _D;
    double _R;

    bool _computed;

    BinaryTree<double> _priceTree;
    BinaryTree<bool> _exerciseTree;

    void buildStockTree(BinaryTree<double>& stockTree) const;

public:
    CRRPricer(Option* option, int depth,
              double asset_price,
              double up, double down,
              double interest_rate);

    // overload for Black-Scholes approximation
    CRRPricer(Option* option, int depth,
              double asset_price,
              double r, double volatility);

    void compute();
    double get(int n, int i);
    bool getExercise(int n, int i);

    double operator()(bool closed_form = false);
};
