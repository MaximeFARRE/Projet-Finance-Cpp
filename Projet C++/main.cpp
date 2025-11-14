#include <iostream>
#include "MT.h"

int main() {
    MT& mt = MT::getInstance();

    std::cout << "Uniformes : ";
    for (int i = 0; i < 5; ++i)
        std::cout << mt.rand_unif() << " ";
    std::cout << "\n";

    std::cout << "Normales  : ";
    for (int i = 0; i < 5; ++i)
        std::cout << mt.rand_norm() << " ";
    std::cout << "\n";

    return 0;
}
