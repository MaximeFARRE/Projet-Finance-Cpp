#pragma once
#include "Option.h"
#include "BinaryTree.h"

// CRR pricer for European and American options
class CRRPricer {
private:
    Option* _option;
    int _depth;

    // Model parameters
    double _S0;
    double _U;
    double _D;
    double _R;

    bool _computed;

    // Trees for option prices and exercise policy
    BinaryTree<double> _priceTree;
    BinaryTree<bool> _exerciseTree;

    // Build the stock price tree 
    void buildStockTree(BinaryTree<double>& stockTree) const;

public:
    // Constructor with explicit CRR parameters
    CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate);

    // Constructor using Black-Scholes parameters
    CRRPricer(Option* option, int depth, double asset_price, double r, double volatility);

    // Compute the price tree
    void compute();
    // Get 
    double get(int n, int i);
    bool getExercise(int n, int i);

    // Return the price
    double operator()(bool closed_form = false);
};
