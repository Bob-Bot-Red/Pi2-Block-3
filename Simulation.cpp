#include "Simulation.h"
#include "Fahrrad.h"
#include "Kreuzung.h"
#include "PKW.h"
#include "Tempolimit.h"

#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

extern double dEpsilon;
extern double dGlobaleZeit;

namespace
{
    void vPruefezeilenende(std::istringstream &zeile)
    {
        std::string rest;
        if (zeile >> rest)
            throw std::runtime_error("Unerwartete Zusatzdaten");
    }

    Tempolimit eTempoLimitAusZahl(int iTempolimit)
    {
        switch (iTempolimit)
        {
        case 1:
            return Tempolimit::Innerorts;
        case 2:
            return Tempolimit::Landstrasse;
        case 3:
            return Tempolimit::Autobahn;
        default:
            throw std::runtime_error("Tempolimit muss = 1, 2, 3 sein");
        }
    }
}

std::shared_ptr<Kreuzung> Simulation::pKreuzungFinden(const std::string &sName) const
{
    auto it = p_pKreuzungen.find(sName);
    if (it == p_pKreuzungen.end())
        throw std::runtime_error("kreuzung nicht gefunden: " + sName);
    return it->second;
}

void Simulation::vEinlesen(std::istream &in)
{
    std::string sZeile;
    std::size_t izeile = 0;

    try
    {
        while (std::getline(in, sZeile))
        {
            ++izeile;
            std::istringstream zeile(sZeile);
            std::string sTyp;
            if (!(zeile >> sTyp))
                continue;

            if (sTyp == "KREUZUNG")
            {
                auto pKreuzung = std::make_shared<Kreuzung>("");
                zeile >> *pKreuzung;
                vPruefezeilenende(zeile);

                auto ergebnis = p_pKreuzungen.emplace(pKreuzung->getName(), pKreuzung);

                if (!ergebnis.second)
                {
                    throw std::runtime_error("kreuzung nicht gefunden: " + pKreuzung->getName());
                }
            }
            else if (sTyp == "STRASSE")
            {
                std::string sQuelle, sZiel, sHin, sRueck;
                double dLaenge;
                int iLimit, iVerbot;

                if (!(zeile >> sQuelle >> sZiel >> sHin >> sRueck >> dLaenge >> iLimit >> iVerbot))
                    throw std::runtime_error("Strasse Info fehlt");

                vPruefezeilenende(zeile);

                if (iVerbot != 0 && iVerbot != 1)
                    throw std::runtime_error("Überholverbot muss = 0/1 sein");

                Kreuzung::vVerbinde(sHin, sRueck, dLaenge, pKreuzungFinden(sQuelle), pKreuzungFinden(sZiel), eTempoLimitAusZahl(iLimit), iVerbot == 1);
            }
            else if (sTyp == "PKW")
            {
                auto pFahrzeug = std::make_unique<PKW>();
                zeile >> *pFahrzeug;

                std::string sStart;
                double dStartZeit;
                if (!(zeile >> sStart >> dStartZeit))
                    throw std::runtime_error("PKW Info fehlen");

                vPruefezeilenende(zeile);
                pKreuzungFinden(sStart)->vAnnahme(std::move(pFahrzeug), dStartZeit);
            }
            else if (sTyp == "FAHRRAD")
            {
                auto pFahrzeug = std::make_unique<Fahrrad>();
                zeile >> *pFahrzeug;

                std::string sStart;
                double dStartzeit;
                if (!(zeile >> sStart >> dStartzeit))
                    throw std::runtime_error("Fahrrad Info fehlen");
                vPruefezeilenende(zeile);
                pKreuzungFinden(sStart)->vAnnahme(std::move(pFahrzeug), dStartzeit);
            }
            else
            {
                throw std::runtime_error("unbekannte Eingabe" + sTyp);
            }
        }
    }

    asda
    catch (const std::exception &e)
    {

        std::cerr << "Fehlet in Zeile " << izeile << ": " << e.what() << '\n';
        throw;
    }
}

void Simulation::vSimulieren(double dDauer, double dZeitschritt)
{
    if (dDauer < 0.0 || dZeitschritt <= 0.0)
        throw std::invalid_argument("Dauer muss >= 0 und Zeitschritt muss > 0 sein");

    const double dStartzeit = dGlobaleZeit;
    const auto iSchritte = static_cast<std::size_t>(std::floor((dDauer + dEpsilon) / dZeitschritt));

    for (std::size_t i = 0; i <= iSchritte; ++i)
    {
        dGlobaleZeit = dStartzeit + i * dZeitschritt;
        for (const auto &paar : p_pKreuzungen)
            paar.second->vSimulieren();
    }
}
