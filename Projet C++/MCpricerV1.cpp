#include <iostream>
#include <cmath>
#include <random>

double prix_call_mc(double S0, double K, double r, double sigma, double T, int N)
{
    // generateur aleatoire
    std::mt19937 gen(110); // Mersenne Twister
    std::normal_distribution<double> normal(0.0, 1.0);

    double sommePayoff = 0;

    for (int i = 0; i < N; i++)
    {
        double Z = normal(gen); // variable normale

        // simulation de S_T avec Black Scholes
        double ST = S0 * exp((r - 0.5 * sigma * sigma) * T + sigma * sqrt(T) * Z);

        // payoff du call
        double payoff = ST - K;
        if (payoff < 0)
            payoff = 0;

        sommePayoff = sommePayoff + payoff;
    }

    double moyenne = sommePayoff / N;

    // actualisation
    double prix = exp(-r * T) * moyenne;

    return prix;
}

int main()
{
    double S0 = 100;
    double K = 100;
    double r = 0.05;
    double sigma = 0.2;
    double T = 1.0;
    int N = 100000; // nombre de simulations

    double prix = prix_call_mc(S0, K, r, sigma, T, N);

    std::cout << "prix du call = " << prix << std::endl;

    return 0;
}
