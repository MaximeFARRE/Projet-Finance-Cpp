#include "MT.h"
#include <cmath>

// Constructeur privé : initialise la seed
MT::MT()
{
    std::random_device rd;
    _gen = std::mt19937(rd()); // initialisation du mt19937
}

// Retourne l'unique instance
MT& MT::getInstance()
{
    static MT instance;   // créé une fois, réutilisé ensuite
    return instance;
}

// Génère un uniforme U ~ [0,1]
double MT::rand_unif()
{
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(_gen);
}

// Génère une normale Z ~ N(0,1)
double MT::rand_norm()
{
    static std::normal_distribution<double> dist(0.0, 1.0);
    return dist(_gen);
}
