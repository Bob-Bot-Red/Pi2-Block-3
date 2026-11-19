#ifndef PKW_H
#define PKW_H
#include <limits>

#include "Fahrzeug.h"

class PKW : public Fahrzeug
{
public:
    PKW();
    PKW(const std::string &sName, const double dMaxGeschwindigkeit, const double dVerbrauch, const double dTankvolumen = 55.0);

    virtual ~PKW();

    void vAusgeben(std::ostream &out) const override;
    void vSimulieren() override;
    double dTanken(double dMenge = std::numeric_limits<double>::infinity()) override;
    double dGeschwindigkeit() const override;
    PKW &operator=(const PKW &rhs);
    virtual void vZeichnen(const Weg &FzgWeg) const override;

    void vEinlesen(std::istream &in) override;

private:
    double p_dVerbrauch = 0.0;
    double p_dTankvolumen = 0.0;
    double p_dTankinhalt = 0.0;
};

#endif