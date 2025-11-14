#include <iostream>
#include <iomanip>
#include <vector>

// Options européennes vanilles et digitales
#include "CallOption.h"
#include "PutOption.h"
#include "EuropeanDigitalCallOption.h"
#include "EuropeanDigitalPutOption.h"

// Options américaines
#include "AmericanCallOption.h"
#include "AmericanPutOption.h"

// Options asiatiques
#include "AsianCallOption.h"
#include "AsianPutOption.h"

// Pricers
#include "BlackScholesPricer.h"
#include "BlackScholesMCPricer.h"
#include "CRRPricer.h"

int main() {
    // -----------------------------
    // Paramètres communs (Excel)
    // -----------------------------
    double T = 5.0;
    double S0 = 100.0;
    double r = 0.01;
    double sigma = 0.1;
    double K = 101.0;

    // Paramètres CRR (par pas)
    int    N = 5;
    double R = 0.01;    // taux par pas
    double U = 0.05;    // up
    double D = -0.045;  // down

    long mcPaths = 100000; // nombre de trajectoires Monte Carlo

    std::cout << std::fixed << std::setprecision(6);

    // -----------------------------
    // Création des différentes options
    // -----------------------------
    // Européennes
    CallOption                euroCall(T, K);
    PutOption                 euroPut(T, K);
    EuropeanDigitalCallOption euroDigCall(T, K);
    EuropeanDigitalPutOption  euroDigPut(T, K);

    // Américaines
    AmericanCallOption        amerCall(T, K);
    AmericanPutOption         amerPut(T, K);

    // Asiatiques : on prend N dates régulières (1..N)*T/N
    std::vector<double> timeSteps;
    for (int k = 1; k <= N; ++k) {
        timeSteps.push_back(T * static_cast<double>(k) / static_cast<double>(N));
    }
    AsianCallOption asianCall(timeSteps, K);
    AsianPutOption  asianPut(timeSteps, K);

    // =====================================================
    // 1) CRR : European & American, avec paramètres (R,U,D)
    // =====================================================

    CRRPricer crrEuroCall(&euroCall, N, S0, U, D, R);
    CRRPricer crrEuroDigCall(&euroDigCall, N, S0, U, D, R);
    CRRPricer crrAmerCall(&amerCall, N, S0, U, D, R);
    CRRPricer crrEuroPut(&euroPut, N, S0, U, D, R);
    CRRPricer crrEuroDigPut(&euroDigPut, N, S0, U, D, R);
    CRRPricer crrAmerPut(&amerPut, N, S0, U, D, R);

    double crrEuroCallPrice = crrEuroCall(false);
    double crrEuroDigCallPrice = crrEuroDigCall(false);
    double crrAmerCallPrice = crrAmerCall(false);
    double crrEuroPutPrice = crrEuroPut(false);
    double crrEuroDigPutPrice = crrEuroDigPut(false);
    double crrAmerPutPrice = crrAmerPut(false);

    std::cout << "================ CRR ================\n";
    std::cout << "European Call        : " << crrEuroCallPrice << "\n";
    std::cout << "European Digital Call: " << crrEuroDigCallPrice << "\n";
    std::cout << "American Call        : " << crrAmerCallPrice << "\n";
    std::cout << "European Put         : " << crrEuroPutPrice << "\n";
    std::cout << "European Digital Put : " << crrEuroDigPutPrice << "\n";
    std::cout << "American Put         : " << crrAmerPutPrice << "\n\n";

    // =====================================================
    // 2) BLACK SCHOLES PRICER - Closed Form (Européennes)
    // =====================================================

    BlackScholesPricer bsEuroCall(&euroCall, S0, r, sigma);
    BlackScholesPricer bsEuroDigCall(&euroDigCall, S0, r, sigma);
    BlackScholesPricer bsEuroPut(&euroPut, S0, r, sigma);
    BlackScholesPricer bsEuroDigPut(&euroDigPut, S0, r, sigma);

    double bsEuroCallPrice = bsEuroCall();
    double bsEuroDigCallPrice = bsEuroDigCall();
    double bsEuroPutPrice = bsEuroPut();
    double bsEuroDigPutPrice = bsEuroDigPut();

    std::cout << "====== BLACK SCHOLES PRICER - Closed Form ======\n";
    std::cout << "European Call        : " << bsEuroCallPrice << "\n";
    std::cout << "European Digital Call: " << bsEuroDigCallPrice << "\n";
    std::cout << "European Put         : " << bsEuroPutPrice << "\n";
    std::cout << "European Digital Put : " << bsEuroDigPutPrice << "\n\n";

    // =====================================================
    // 3) MONTE CARLO
    //    European Call, Asian Call, European Digital Call,
    //    European Put, European Digital Put, Asian Put
    // =====================================================

    // European Call
    BlackScholesMCPricer mcEuroCall(&euroCall, S0, r, sigma);
    mcEuroCall.generate(mcPaths);
    double mcEuroCallPrice = mcEuroCall();
    auto   mcEuroCallCI = mcEuroCall.confidenceInterval();

    // Asian Call
    BlackScholesMCPricer mcAsianCall(&asianCall, S0, r, sigma);
    mcAsianCall.generate(mcPaths);
    double mcAsianCallPrice = mcAsianCall();
    auto   mcAsianCallCI = mcAsianCall.confidenceInterval();

    // European Digital Call
    BlackScholesMCPricer mcEuroDigCall(&euroDigCall, S0, r, sigma);
    mcEuroDigCall.generate(mcPaths);
    double mcEuroDigCallPrice = mcEuroDigCall();
    auto   mcEuroDigCallCI = mcEuroDigCall.confidenceInterval();

    // European Put
    BlackScholesMCPricer mcEuroPut(&euroPut, S0, r, sigma);
    mcEuroPut.generate(mcPaths);
    double mcEuroPutPrice = mcEuroPut();
    auto   mcEuroPutCI = mcEuroPut.confidenceInterval();

    // European Digital Put
    BlackScholesMCPricer mcEuroDigPut(&euroDigPut, S0, r, sigma);
    mcEuroDigPut.generate(mcPaths);
    double mcEuroDigPutPrice = mcEuroDigPut();
    auto   mcEuroDigPutCI = mcEuroDigPut.confidenceInterval();

    // Asian Put
    BlackScholesMCPricer mcAsianPut(&asianPut, S0, r, sigma);
    mcAsianPut.generate(mcPaths);
    double mcAsianPutPrice = mcAsianPut();
    auto   mcAsianPutCI = mcAsianPut.confidenceInterval();

    std::cout << "================ MC ================\n";
    std::cout << "European Call        : " << mcEuroCallPrice
        << "   CI95 = [" << mcEuroCallCI[0] << ", " << mcEuroCallCI[1] << "]\n";

    std::cout << "Asian Call           : " << mcAsianCallPrice
        << "   CI95 = [" << mcAsianCallCI[0] << ", " << mcAsianCallCI[1] << "]\n";

    std::cout << "European Digital Call: " << mcEuroDigCallPrice
        << "   CI95 = [" << mcEuroDigCallCI[0] << ", " << mcEuroDigCallCI[1] << "]\n";

    std::cout << "European Put         : " << mcEuroPutPrice
        << "   CI95 = [" << mcEuroPutCI[0] << ", " << mcEuroPutCI[1] << "]\n";

    std::cout << "European Digital Put : " << mcEuroDigPutPrice
        << "   CI95 = [" << mcEuroDigPutCI[0] << ", " << mcEuroDigPutCI[1] << "]\n";

    std::cout << "Asian Put            : " << mcAsianPutPrice
        << "   CI95 = [" << mcAsianPutCI[0] << ", " << mcAsianPutCI[1] << "]\n";

    return 0;
}
