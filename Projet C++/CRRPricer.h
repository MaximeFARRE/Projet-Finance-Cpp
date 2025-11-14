#pragma once

#include "Option.h"
#include "BinaryTree.h"

class CRRPricer {
private:
    Option* _option;          // option à pricer
    int _depth;               // N
    double _S0;               // prix initial
    double _U;                // up
    double _D;                // down
    double _R;                // taux sans risque par pas

    BinaryTree<double> _tree;     // H(n,i)
    BinaryTree<bool>   _exercise; // politique d'exercice pour Américaines

    bool _computed;

    double stockPrice(int n, int i) const;

public:
    // Constructeur "classique" : on donne U, D, R
    CRRPricer(Option* option,
              int depth,
              double asset_price,
              double up,
              double down,
              double interest_rate);

    // Constructeur Black-Scholes approx : on donne r (continu) et sigma
    CRRPricer(Option* option,
              int depth,
              double asset_price,
              double r,
              double volatility);

    // CRR backward induction (Européenne ou Américaine)
    void compute();

    // H(n,i)
    double get(int n, int i) const;

    // Politique d'exercice : true si on exerce à (n,i)
    bool getExercise(int n, int i) const;

    // Prix de l'option :
    //  - Européenne : closed_form=false → CRR, closed_form=true → formule fermée
    //  - Américaine : closed_form doit rester false (sinon on l'ignore / exception)
    double operator()(bool closed_form = false);
};
