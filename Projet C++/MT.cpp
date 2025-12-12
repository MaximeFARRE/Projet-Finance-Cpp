#include "MT.h"
#include <cmath>

// Private constructor
MT::MT()
{
    std::random_device rd;
    _gen = std::mt19937(rd());
}

// Returns the unique instance of MT
MT& MT::getInstance()
{
    static MT instance;
    return instance;
}

// Generate a uniform random variable
double MT::rand_unif()
{
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(_gen);
}

// Generate a normal random variable
double MT::rand_norm()
{
    static std::normal_distribution<double> dist(0.0, 1.0);
    return dist(_gen);
}
