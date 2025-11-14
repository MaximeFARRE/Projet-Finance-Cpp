#pragma once

#include <vector>

// Classe abstraite de base pour toutes les options
class Option {
protected:
    double _expiry;   // maturité T

public:
    explicit Option(double expiry);

    double getExpiry() const;

    // Payoff h(z) de l'option
    virtual double payoff(double z) const = 0;

    // Par défaut : pas asiatique, pas américaine
    virtual bool isAsianOption() const { return false; }
    virtual bool isAmericanOption() const { return false; }

    // Payoff sur un chemin (par défaut : payoff au dernier point)
    virtual double payoffPath(const std::vector<double>& path) const;

    virtual ~Option() = default;
};
