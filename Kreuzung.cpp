#include "Kreuzung.h"
#include "Weg.h"
#include "Fahrzeug.h"

#include <stdexcept>
#include <utility>

Kreuzung::Kreuzung(const std::string &sName, double dTankstelle)
    : Simulationsobjekt(sName), 
    p_dTankstelle(dTankstelle > 0.0 ? dTankstelle : 0.0)
{
}

Kreuzung::~Kreuzung() = default;

void Kreuzung::vVerbinde( const std::string &sNameHin, const std::string &sNameRueck, double dLaenge, const std::shared_ptr<Kreuzung> &pStart, const std::shared_ptr<Kreuzung> &pZiel, Tempolimit eTempolimit, bool bUeberholverbot)
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
    if(p_dTankstelle <=0.0)
    return;

    const double dGetankt = fahrzeug.dTanken();
    p_dTankstelle -= dGetankt;

    if(p_dTankstelle < 0.0)
    {
        p_dTankstelle = 0.0;
    }
}


void Kreuzung::vAnnahme(std::unique_ptr<Fahrzeug> fahrzeug, double dStartzeit)
{
    if(!fahrzeug)
    return;

    if(p_pWeg.empty())
    throw std::logic_error("vAnnahme: Kreuzung hat keinen Weg");
    
    vTanken(*fahrzeug);

    p_pWeg.front()->vAnnahme(std::move(fahrzeug), dStartzeit);
}


void Kreuzung::vSimulieren()
{
    for(const auto &pWeg : p_pWeg)
    pWeg->vSimulieren();
}