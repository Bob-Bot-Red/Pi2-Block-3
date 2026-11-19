#include "Streckenende.h"
#include "Fahrzeug.h"
#include "Weg.h"
#include "Kreuzung.h"
#include <utility>
#include <iomanip>
#include <iostream>

extern double dGlobaleZeit;

Streckenende::Streckenende(Fahrzeug &fahrzeug, Weg &weg)
    : Fahrausnahme(fahrzeug, weg)
{
}
/*
//Streckenende::vBearbeiten()
{
    std::cout << "Streckenende: Fahrzeug " << p_pFahrzeug.getName()
              << " auf Weg " << p_pWeg.getName() << std::endl;
    Fahrzeug *pFahrzeug = &p_pFahrzeug;
    Weg *pWeg = &p_pWeg;

    p_pWeg.vVerzoegert([pFahrzeug, pWeg]()
                       { auto pAbgegebenesFahrzeug = pWeg->pAbgabe(*pFahrzeug); });
} */

void Streckenende::vBearbeiten()
{
    Fahrzeug *pFahrzeugroh = &p_pFahrzeug;
    Weg *pAlterWeg = &p_pWeg;

    const std::shared_ptr<Kreuzung> pKreuzung = pAlterWeg->getZielKreuzung();
    if (!pKreuzung)
    {
        pAlterWeg->vVerzoegert([pFahrzeugroh, pAlterWeg]()
                               { auto pAbfahrendesFahrzeug = pAlterWeg->pAbgabe(*pFahrzeugroh); });
        return;
    }

    pAlterWeg->vVerzoegert([pFahrzeugroh, pAlterWeg, pKreuzung]()
                           {
                               auto pAbgegebenesFahrzeug = pAlterWeg->pAbgabe(*pFahrzeugroh);
                               if (!pAbgegebenesFahrzeug)
                                   return;

                               auto pNeuerWeg = pKreuzung->pZufaelligerWeg(*pAlterWeg);

                               pKreuzung->vTanken(*pAbgegebenesFahrzeug);

                               std::cout << std::resetiosflags(std::ios::right) << std::setiosflags(std::ios::left)
                                         << std::setw(10) << "Zeit" << ":" << dGlobaleZeit << '\n'
                                         << std::setw(10) << "Kreuzung " << ":" << pKreuzung->getName() << " " << pKreuzung->getTankstelle() << "\n"
                                         << std::setw(10) <<"Wechsel" << ":" << pAlterWeg->getName() << " -> "<< pNeuerWeg->getName() 
                                         << std::setw(10)<< "Fahrzeug: " << ":" << *pAbgegebenesFahrzeug << "\n";
                               pNeuerWeg->vAnnahme(std::move(pAbgegebenesFahrzeug)); });
}