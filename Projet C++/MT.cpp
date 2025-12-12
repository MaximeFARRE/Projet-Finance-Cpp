#include "MT.h"
#include <ctime>

std::mt19937 MT::_gen;
bool MT::_isInit = false;

void MT::init() {
    if (!_isInit) {
        unsigned int seed = static_cast<unsigned int>(std::time(nullptr));
        _gen.seed(seed);
        _isInit = true;
    }
}

double MT::rand_unif() {
    init();
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(_gen);
}

double MT::rand_norm() {
    init();
    std::normal_distribution<double> dist(0.0, 1.0);
    return dist(_gen);
}
