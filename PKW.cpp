#include "PKW.h"
#include <limits>
#include <iomanip>
#include "Weg.h"
#include <iostream>
#include <algorithm>
#include "SimuClient.h"
#include <stdexcept>

extern double dGlobaleZeit;

PKW::PKW()
{
}

PKW::PKW(const std::string &sName, const double dMaxGeschwindigkeit, const double dVerbrauch, const double dTankvolumen)
    : Fahrzeug(sName, dMaxGeschwindigkeit),
      p_dVerbrauch(dVerbrauch),
      p_dTankvolumen(dTankvolumen > 0.0 ? dTankvolumen : 0.0),
      p_dTankinhalt(p_dTankvolumen / 2.0)
{
}

PKW::~PKW()
{
}

double PKW::dTanken(const double dMenge)
{
    double dFreierPlatz = p_dTankvolumen - p_dTankinhalt;
    double dGetankteMenge = 0.0;
    if (dMenge == std::numeric_limits<double>::infinity())
    {
        dGetankteMenge = dFreierPlatz;
    }
    else if (dMenge > 0.0)
    {
        dGetankteMenge = (dMenge <= dFreierPlatz) ? dMenge : dFreierPlatz;
    }
    p_dTankinhalt += dGetankteMenge;
    return dGetankteMenge;
}

void PKW::vSimulieren()
{
    if (p_dTankinhalt > 0.0)
    {
        double dStreckeVorher = p_dGesamtStrecke;
        Fahrzeug::vSimulieren();
        double dGefahreneStrecke = p_dGesamtStrecke - dStreckeVorher;
        double dVerbrauchInSchritt = (dGefahreneStrecke / 100.0) * p_dVerbrauch;

        p_dTankinhalt -= dVerbrauchInSchritt;
        if (p_dTankinhalt < 0.0)
        {
            p_dTankinhalt = 0.0;
        }
    }
    else
    {
        p_dZeit = dGlobaleZeit;
    }
}

void PKW::vAusgeben(std::ostream &out) const
{
    Fahrzeug::vAusgeben(out);

    double dGesamtverbrauch = (p_dGesamtStrecke / 100.00) * p_dVerbrauch;

    out << std::setw(20) << dGesamtverbrauch
        << std::setw(20) << p_dTankinhalt;
}

PKW &PKW::operator=(const PKW &rhs)
{
    if (this != &rhs)
    {
        Fahrzeug::operator=(rhs);
        p_dVerbrauch = rhs.p_dVerbrauch;
        p_dTankvolumen = rhs.p_dTankvolumen;
        p_dTankinhalt = rhs.p_dTankinhalt;
    }
    return *this;
}

double PKW::dGeschwindigkeit() const
{
    if (!p_pVerhalten)
    {
        return p_dMaxGeschwindigkeit;
    }
    return std::min(p_dMaxGeschwindigkeit, p_pVerhalten->getWeg().getTempolimit());
}

void PKW::vZeichnen(const Weg &FzgWeg) const
{
    const double dRelPosition = p_dAbschnittStrecke / FzgWeg.getLaenge();
    bZeichnePKW(getName(), FzgWeg.getName(), dRelPosition, dGeschwindigkeit(), p_dTankinhalt);
}

void PKW::vEinlesen(std::istream &in)
{
    Fahrzeug::vEinlesen(in);
    if (!(in >> p_dVerbrauch >> p_dTankvolumen))
        throw std::runtime_error("Verbrauch | Volumen vom PKW nicht einlesbr");

    if (p_dTankvolumen < 0.0)
    {
        p_dTankvolumen = 0.0;
    }

    p_dTankinhalt = p_dTankvolumen / 2.0;
}