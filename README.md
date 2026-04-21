# Financial Options Pricing Library — C++

A C++ library implementing pricing models for European, American, and Asian financial options, developed as part of an applied quantitative finance course at [ESILV](https://www.esilv.fr/) (École Supérieure d'Ingénieurs Léonard de Vinci).

---

## Overview

This project implements three standard option pricing methodologies and applies them to four families of derivative instruments. The goal is to provide a clean, object-oriented implementation that reflects the mathematical models taught in quantitative finance courses, with emphasis on correctness, extensibility, and numerical performance.

---

## Features

### Option types supported

| Family | Variants |
|---|---|
| European Vanilla | Call, Put |
| European Digital (Binary) | Call, Put |
| American | Call, Put |
| Asian (arithmetic average) | Call, Put |

### Pricing models

| Model | Applicable options | Method |
|---|---|---|
| **Black-Scholes** (closed form) | European Vanilla, European Digital | Analytical formula with `N(d1)`, `N(d2)` |
| **Black-Scholes Monte Carlo** | All option types (via path simulation) | GBM path simulation with Mersenne Twister RNG |
| **Cox-Ross-Rubinstein (CRR)** | European, American | Binomial tree with backward induction and early exercise |

### Additional capabilities

- Delta computation for European options (Black-Scholes)
- 95% confidence interval on Monte Carlo estimates
- CRR pricer callable in closed-form approximation mode (via Black-Scholes parameter conversion)
- Early exercise detection stored in a binary exercise tree (American options)
- Path-dependent payoff support for Asian options (arithmetic average)
- Execution time measurement

---

## Project structure

```
Projet-Finance-Cpp/
├── Projet C++/                  # All source files
│   ├── Option.h / .cpp          # Abstract base class for all options
│   │
│   ├── EuropeanVanillaOption.h / .cpp   # European vanilla base
│   ├── CallOption.h / .cpp              # European vanilla call
│   ├── PutOption.h / .cpp               # European vanilla put
│   │
│   ├── EuropeanDigitalOption.h / .cpp   # European digital base
│   ├── EuropeanDigitalCallOption.h / .cpp
│   ├── EuropeanDigitalPutOption.h / .cpp
│   │
│   ├── AmericanOption.h / .cpp          # American option base
│   ├── AmericanCallOption.h / .cpp
│   ├── AmericanPutOption.h / .cpp
│   │
│   ├── AsianOption.h / .cpp             # Asian option base (path-dependent)
│   ├── AsianCallOption.h / .cpp
│   ├── AsianPutOption.h / .cpp
│   │
│   ├── BlackScholesPricer.h / .cpp      # Closed-form BS pricer
│   ├── BlackScholesMCPricer.h / .cpp    # Monte Carlo BS pricer
│   ├── CRRPricer.h / .cpp               # CRR binomial tree pricer
│   │
│   ├── BinaryTree.h / .cpp              # Generic template binary tree
│   ├── MT.h / .cpp                      # Mersenne Twister RNG (singleton)
│   │
│   └── main.cpp                         # Entry point with usage examples
│
├── Projet C++.sln                       # Visual Studio solution file
└── Projet C++.vcxproj                   # Visual Studio project file
```

---

## Architecture

The design follows a clean polymorphic hierarchy:

```
Option  (abstract)
├── EuropeanVanillaOption  (abstract)
│   ├── CallOption
│   └── PutOption
├── EuropeanDigitalOption  (abstract)
│   ├── EuropeanDigitalCallOption
│   └── EuropeanDigitalPutOption
├── AmericanOption  (abstract)
│   ├── AmericanCallOption
│   └── AmericanPutOption
└── AsianOption  (abstract, path-dependent)
    ├── AsianCallOption
    └── AsianPutOption

Pricers  (independent, accept Option*)
├── BlackScholesPricer      — closed-form, European only
├── BlackScholesMCPricer    — simulation, all option types
└── CRRPricer               — binomial tree, European and American

Utilities
├── BinaryTree<T>           — generic template tree used by CRR
└── MT                      — Mersenne Twister singleton (thread-local RNG)
```

Pricers are decoupled from the option hierarchy and interact through the `Option` abstract interface. Path-dependent options override `payoffPath()` to provide custom averaging logic consumed by the Monte Carlo pricer.

---

## Requirements

- C++17 or later
- Visual Studio 2019 / 2022 (Windows) — project files included
- Standard library only — no external dependencies

---

## Build and run

### Visual Studio (Windows)

1. Open `Projet C++.sln` in Visual Studio.
2. Select the desired configuration (`Debug` or `Release`).
3. Build the solution (`Ctrl+Shift+B`).
4. Run the executable (`F5` or `Ctrl+F5`).

### Command line (GCC / Clang)

```bash
# From the "Projet C++/" directory
g++ -std=c++17 -O2 -o pricer \
    Option.cpp \
    CallOption.cpp PutOption.cpp EuropeanVanillaOption.cpp \
    EuropeanDigitalOption.cpp EuropeanDigitalCallOption.cpp EuropeanDigitalPutOption.cpp \
    AmericanOption.cpp AmericanCallOption.cpp AmericanPutOption.cpp \
    AsianOption.cpp AsianCallOption.cpp AsianPutOption.cpp \
    BlackScholesPricer.cpp BlackScholesMCPricer.cpp \
    CRRPricer.cpp MT.cpp BinaryTree.cpp \
    main.cpp

./pricer
```

---

## Usage

The entry point `main.cpp` demonstrates how to instantiate options and compute prices.

### Example — European call (Monte Carlo)

```cpp
double T     = 5.0;    // maturity (years)
double K     = 101.0;  // strike price
double S0    = 100.0;  // initial spot price
double r     = 0.01;   // risk-free rate
double sigma = 0.10;   // volatility

CallOption euroCall(T, K);

BlackScholesMCPricer mcPricer(&euroCall, S0, r, sigma);
mcPricer.generate(1'000'000);

double price = mcPricer();
auto ci      = mcPricer.confidenceInterval();   // 95% CI

std::cout << "Price : " << price << "\n";
std::cout << "CI95% : [" << ci[0] << ", " << ci[1] << "]\n";
```

### Example — European call (Black-Scholes closed form)

```cpp
BlackScholesPricer bsPricer(&euroCall, S0, r, sigma);
double price = bsPricer();
double delta = bsPricer.delta();
```

### Example — American put (CRR binomial tree)

```cpp
int    N = 5;
double U = 0.05;    // up factor
double D = -0.045;  // down factor
double R = 0.01;    // per-step interest rate

AmericanPutOption amerPut(T, K);
CRRPricer crrPricer(&amerPut, N, S0, U, D, R);
double price = crrPricer(false);   // false = standard tree, true = closed-form approx
```

### Example — Asian call (Monte Carlo)

```cpp
int N = 5;
std::vector<double> timeSteps;
for (int k = 1; k <= N; ++k)
    timeSteps.push_back(T * k / N);

AsianCallOption asianCall(timeSteps, K);

BlackScholesMCPricer mcAsian(&asianCall, S0, r, sigma);
mcAsian.generate(1'000'000);
double price = mcAsian();
```

---

## Sample output

```
European Call IC95% : [12.043218, 12.052781]

================ MC (BlackScholesMCPricer) ================
European Call        : 12.047999

Execution time: 1.823412 seconds
```

---

## Known limitations

- The CRR pricer does not support Asian options (path dependency is incompatible with standard binomial trees).
- The Black-Scholes closed-form pricer supports European options only; American and Asian options require the Monte Carlo pricer.
- The Monte Carlo pricer currently uses a fixed number of paths per `generate()` call; incremental simulation is supported by calling `generate()` multiple times.
- No CMake build system is provided; compilation on non-Windows platforms requires manual file listing (see command-line build instructions above).
- The random number generator uses a fixed initial seed; results are deterministic across runs with the same seed.

---

## Contributors

| Name | GitHub | Role |
|---|---|---|
| Maxime Farré | [@MaximeFARRE](https://github.com/MaximeFARRE) | Asian options, Monte Carlo pricer, project integration |
| Chloé Chaufour | [@chloe28072004](https://github.com/chloe28072004) | American options, CRR pricer, Black-Scholes pricer fix |
| Mathilde Destailleur | [@mathildedest](https://github.com/mathildedest) | BinaryTree, CRR pricer |
| Auriane Carron | [@AurianeCarron73](https://github.com/AurianeCarron73) | European options base |
| Emilien Combaret | [@EmilienCbrt](https://github.com/EmilienCbrt) · [@EmilienCombaret](https://github.com/EmilienCombaret) | Code review, documentation |

---

## Academic context

This project was developed as part of the **C++ Programming** course at ESILV (École Supérieure d'Ingénieurs Léonard de Vinci), in the 4th year of the engineering program (semester 7), academic year 2025–2026.

The project demonstrates:
- Object-oriented design with abstract base classes and polymorphism
- Template programming (`BinaryTree<T>`)
- Numerical methods in finance (Monte Carlo simulation, binomial tree)
- Analytical option pricing (Black-Scholes formula with normal CDF/PDF)
- Standard C++ library usage (`<vector>`, `<chrono>`, `<cmath>`, `<iostream>`)

---

## Project status

**Complete** — submitted December 2025. No active development planned.
