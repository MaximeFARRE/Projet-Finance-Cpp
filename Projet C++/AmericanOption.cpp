#include "AmericanOption.h"
#include <stdexcept>

using namespace std;

// Contructor
AmericanOption::AmericanOption(double expiry, double strike) : Option(expiry), _strike(strike) {
    // Check that the strike is valid
    if (strike < 0.0) {
        throw invalid_argument("strike must be non negative");
    }
}
// This method indicates that the option is American or not
bool AmericanOption::isAmericanOption() const {
    return true;
}

// Getter 
double AmericanOption::getStrike() const {
    return _strike;
}