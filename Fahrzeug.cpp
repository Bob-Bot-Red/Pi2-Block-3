#include "Fahrzeug.h"
#include <iomanip>
#include "Streckenende.h"
#include <stdexcept>

extern double dGlobaleZeit;

Fahrzeug::Fahrzeug()
{
}

Fahrzeug::Fahrzeug(const std::string &sName)
    : Simulationsobjekt(sName)
{
}

Fahrzeug::Fahrzeug(const std::string &sName, const double dMaxGeschwindigkeit)
    : Simulationsobjekt(sName),
      p_dMaxGeschwindigkeit((dMaxGeschwindigkeit > 0.0) ? dMaxGeschwindigkeit : 0.0)
{
    std::cout << " Fahrzeug " << sName << " mit Geschw " << p_dMaxGeschwindigkeit << " wird erstellt " << std::endl;
}

Fahrzeug::~Fahrzeug()
{
}

void Fahrzeug::vKopf()
{
    std::cout << std::resetiosflags(std::ios::right) << std::setiosflags(std::ios::left)
              << std::setw(5) << "ID"
              << std::setw(15) << "Name"
              << std::setw(20) << "MaxGeschwindigkeit"
              << std::setw(20) << "GesamtStrecke"
              << std::setw(20) << "GesamtZeit"
              << std::setw(20) << "AbschnittStrecke"
              << std::setw(20) << "GesamtVerbrauch"
              << std::setw(15) << "Tankinhalt" << std::endl
              << std::setfill('-') << std::setw(130) << "" << std::endl
              << std::setfill(' ');
}

void Fahrzeug::vAusgeben(std::ostream &out) const
{
    Simulationsobjekt::vAusgeben(out);

    out << std::setw(20) << p_dMaxGeschwindigkeit
        << std::setw(20) << p_dGesamtStrecke
        << std::setw(20) << p_dGesamtZeit
        << std::setw(20) << p_dAbschnittStrecke;
}

void Fahrzeug::vSimulieren()
{
    double dZeitDifferenz = dGlobaleZeit - p_dZeit;

    if (dZeitDifferenz <= 0.0)
    {
        return;
    }
    double dTeilStrecke = 0.0;

    if (p_pVerhalten)
    {

        dTeilStrecke = p_pVerhalten->dStrecke(*this, dZeitDifferenz);
        p_dAbschnittStrecke += dTeilStrecke;
    }
    else
    {
        dTeilStrecke = dGeschwindigkeit() * dZeitDifferenz;
    }

    p_dGesamtStrecke += dTeilStrecke;
    p_dGesamtZeit += dZeitDifferenz;
    p_dZeit = dGlobaleZeit;
}

double Fahrzeug::dTanken(const double dMenge)
{
    return 0.0;
}

double Fahrzeug::dGeschwindigkeit() const
{
    return p_dMaxGeschwindigkeit;
}

bool Fahrzeug::operator<(const Fahrzeug &rhs) const
{
    return this->p_dGesamtStrecke < rhs.p_dGesamtStrecke;
}

Fahrzeug &Fahrzeug::operator=(const Fahrzeug &rhs)
{
    if (this != &rhs)
    {
        Simulationsobjekt::operator=(rhs);
        p_dMaxGeschwindigkeit = rhs.p_dMaxGeschwindigkeit;
    }
    return *this;
}

double Fahrzeug::getMaxGesch()
{
    return p_dMaxGeschwindigkeit;
}

void Fahrzeug::vNeueStrecke(Weg &weg)
{

    p_pVerhalten = std::make_unique<Fahren>(weg);
    p_dAbschnittStrecke = 0.0;
    p_dZeit = dGlobaleZeit;
}

void Fahrzeug::vNeueStrecke(Weg &weg, double dStartZeitpunkt)
{

    p_pVerhalten = std::make_unique<Parken>(weg, dStartZeitpunkt);
    p_dAbschnittStrecke = 0.0;
    p_dZeit = dGlobaleZeit;
}

double Fahrzeug::getAbschnittstrecke() const
{
    return p_dAbschnittStrecke;
}

void Fahrzeug::vZeichnen(const Weg &FzgWeg) const
{
}

void Fahrzeug::vEinlesen(std::istream &in)
{
    Simulationsobjekt::vEinlesen(in);
    if(!(in >> p_dMaxGeschwindigkeit))
    throw std::runtime_error("Maxgesch. nicht einlesbar");

    if(p_dMaxGeschwindigkeit < 0.0);
    p_dMaxGeschwindigkeit = 0.0;
}