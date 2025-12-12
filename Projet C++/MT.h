#pragma once
#include <random>

// Singleton class for generating random numbers
class MT {
private:
    std::mt19937 _gen;   // Random number generator

    // Private constructor
    MT();

public:
    static MT& getInstance();

    // Disable copy and assignment
    MT(const MT&) = delete;
    MT& operator=(const MT&) = delete;

    // Disable moving
    MT(MT&&) = delete;
    MT& operator=(MT&&) = delete;

    // Generate a uniform random variable 
    double rand_unif();

    // Generate a standard normal random variable
    double rand_norm();
};
//