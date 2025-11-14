#pragma once

#include <random>

// Singleton MT : générateur Mersenne-Twister unique dans tout le projet
class MT {
private:
    // Le générateur aléatoire
    std::mt19937 _gen;

    // Constructeur privé : empêche la création directe d'objets MT
    MT();

public:
    // Accès à l'unique instance (Singleton)
    static MT& getInstance();

    // Interdit la copie
    MT(const MT&) = delete;
    MT& operator=(const MT&) = delete;

    // Interdit le déplacement
    MT(MT&&) = delete;
    MT& operator=(MT&&) = delete;

    // Génère un uniforme U ~ [0,1]
    double rand_unif();

    // Génère une normale Z ~ N(0,1)
    double rand_norm();
};
