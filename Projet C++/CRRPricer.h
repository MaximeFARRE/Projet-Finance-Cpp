#pragma once

#include "Option.h"
#include "BinaryTree.h"

// CRRPricer : pricer d'options européennes avec le modèle binomial CRR
class CRRPricer {
private:
    Option* _option;          // pointeur vers l'option à pricer
    int _depth;               // N
    double _S0;               // prix initial de l'actif
    double _U;                // up  = U
    double _D;                // down = D
    double _R;                // taux sans risque R

    BinaryTree<double> _tree; // arbre des valeurs H(n,i)

    bool _computed;           // indique si compute() a déjà été fait

    // fonction utilitaire : calcule le prix S(n,i)
    double stockPrice(int n, int i) const;

public:
    // Constructeur : 
    // depth = N, asset_price = S0, up = U, down = D, interest_rate = R
    CRRPricer(Option* option,
        int depth,
        double asset_price,
        double up,
        double down,
        double interest_rate);

    // Lance le schéma CRR par backward induction
    void compute();

    // Getter : renvoie H(n,i)
    double get(int n, int i) const;

    // Prix de l'option :
    // - si closed_form=false : utilise compute() et H(0,0)
    // - si closed_form=true : utilise directement la formule fermée
    double operator()(bool closed_form = false);
};
