#pragma once
#include <random>

class MT {
private:
    static std::mt19937 _gen;
    static bool _isInit;

    static void init();

public:
    static double rand_unif();
    static double rand_norm();
};
//