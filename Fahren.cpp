#include "Fahren.h"
#include "Fahrzeug.h"
#include "Weg.h"
#include "Streckenende.h"

#include <algorithm>

Fahren::Fahren(Weg &pWeg)
    : Verhalten(pWeg)
{
}

double Fahren::dStrecke(Fahrzeug &aFzg, double dZeitIntervall) const
{
    const double dPosition = aFzg.getAbschnittstrecke();
    const double dRestBisWegende = getWeg().getLaenge() - dPosition;


    if (dRestBisWegende <= 0.0)
    {
        throw Streckenende(aFzg, getWeg());
    }

    const double dRestbisSchrancke = getWeg().getVirtuelleSchranke() - dPosition;

    if(dRestbisSchrancke <= 0.0)
    {
        return 0.0;
    }

    const double dMoeglicheStrecke = aFzg.dGeschwindigkeit() * dZeitIntervall;
    return std::min(dMoeglicheStrecke, std::min(dRestBisWegende, dRestbisSchrancke));
}
