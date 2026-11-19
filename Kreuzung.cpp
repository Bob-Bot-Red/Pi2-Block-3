#include "Kreuzung.h"
#include "Weg.h"
#include "Fahrzeug.h"

#include <stdexcept>
#include <utility>
#include <random>
#include <vector>
#include <iomanip>

Kreuzung::Kreuzung(const std::string &sName, double dTankstelle)
    : Simulationsobjekt(sName),
      p_dTankstelle(dTankstelle > 0.0 ? dTankstelle : 0.0)
{
}

Kreuzung::~Kreuzung() = default;

void Kreuzung::vVerbinde(const std::string &sNameHin, const std::string &sNameRueck, double dLaenge, const std::shared_ptr<Kreuzung> &pStart, const std::shared_ptr<Kreuzung> &pZiel, Tempolimit eTempolimit, bool bUeberholverbot)
{
    if (!pStart || !pZiel)
    {
        throw std::invalid_argument("vVerbinde: Start + -Zielkreuzung muessen existieren");
    }
    auto pHinweg = std::make_shared<Weg>(sNameHin, dLaenge, eTempolimit, bUeberholverbot, pZiel);
    auto pRueckweg = std::make_shared<Weg>(sNameRueck, dLaenge, eTempolimit, bUeberholverbot, pStart);
    pHinweg->setRueckweg(pRueckweg);
    pRueckweg->setRueckweg(pHinweg);
    pStart->p_pWeg.push_back(pHinweg);
    pZiel->p_pWeg.push_back(pRueckweg);
}

void Kreuzung::vTanken(Fahrzeug &fahrzeug)
{
    if (p_dTankstelle <= 0.0)
        return;

    const double dGetankt = fahrzeug.dTanken();
    p_dTankstelle -= dGetankt;

    if (p_dTankstelle < 0.0)
    {
        p_dTankstelle = 0.0;
    }
}

void Kreuzung::vAnnahme(std::unique_ptr<Fahrzeug> fahrzeug, double dStartzeit)
{
    if (!fahrzeug)
        return;

    if (p_pWeg.empty())
        throw std::logic_error("vAnnahme: Kreuzung hat keinen Weg");

    vTanken(*fahrzeug);

    p_pWeg.front()->vAnnahme(std::move(fahrzeug), dStartzeit);
}

void Kreuzung::vSimulieren()
{
    for (const auto &pWeg : p_pWeg)
        pWeg->vSimulieren();
}

std::shared_ptr<Weg> Kreuzung::pZufaelligerWeg(Weg &ankuftsWeg) const
{
    const std::shared_ptr<Weg> pRueckweg = ankuftsWeg.getRueckweg();
    std::vector<std::shared_ptr<Weg>> vMoeglicheWege;

    for (const auto &pWeg : p_pWeg)
    {
        if (pWeg && pWeg != pRueckweg)
        {
            vMoeglicheWege.push_back(pWeg);
        }
    }
    if (vMoeglicheWege.empty())
    {
        if (pRueckweg)
            return pRueckweg;

        throw std::logic_error("Kreuzung hat keinen passenden Weg & Rueckweg");
    }
    static std::mt19937 zufallsgen(std::random_device{}());
    std::uniform_int_distribution<std::size_t> verteilung(0, vMoeglicheWege.size() - 1);

    return vMoeglicheWege[verteilung(zufallsgen)];
}

double Kreuzung::getTankstelle() const
{
    return p_dTankstelle;
}

void Kreuzung::vEinlesen(std::istream &in)
{
    Simulationsobjekt::vEinlesen(in);

    if (!(in >> p_dTankstelle))
        throw std::runtime_error("Tankstelleninhalt nicht einlesbar");

    if (p_dTankstelle < 0.0)
        p_dTankstelle = 0.0;
}

void Kreuzung::vAusgeben(std::ostream &out) const
{
    Simulationsobjekt::vAusgeben(out);
    out << std::setw(20) << p_dTankstelle;
}