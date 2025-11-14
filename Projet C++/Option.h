#pragma once

#include <vector>   // pour std::vector

// Classe abstraite de base pour toutes les options
class Option {
protected:
    double _expiry;   // maturité T

public:
    // Constructeur
    explicit Option(double expiry);

    // Getter sur la maturité
    double getExpiry() const;

    // Payoff h(z) = fonction abstraite
    virtual double payoff(double z) const = 0;

    // Par défaut, une option n'est PAS asiatique
    virtual bool isAsianOption() const { return false; }

    // Payoff sur un chemin : par défaut, on prend le dernier point
    virtual double payoffPath(const std::vector<double>& path) const;

    // Destructeur virtuel
    virtual ~Option() = default;
};
