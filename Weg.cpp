#include "Weg.h"
#include "Fahrzeug.h"
#include <iomanip>
#include <utility>
#include "Fahrausnahme.h"

Weg::Weg()
    : Weg("", 0.0, Tempolimit::Autobahn)
{
}

Weg::Weg(const std::string &sName, double dLaenge, Tempolimit eTempolimit, bool Ueberholverbot, std::shared_ptr<Kreuzung> pZielkreuzung)
    : Simulationsobjekt(sName),
      p_dLaenge(dLaenge), p_eTempolimit(eTempolimit), p_bUeberholverbot(Ueberholverbot), p_dVirtuelleSchranke(dLaenge), p_pZielkreuzung(pZielkreuzung)
{
}

Weg::~Weg()
{
}

void Weg::vSimulieren()
{

    p_pFahrzeuge.vAktualisieren();

    p_dVirtuelleSchranke = p_dLaenge;

    for (const auto &Autos : p_pFahrzeuge)
    {
        if (!Autos)
        {
            continue;
        }

        const double dAbschnittVorher = Autos->getAbschnittstrecke();
        try
        {
            Autos->vSimulieren();
        }
        catch (Fahrausnahme &ausnahme)
        {
            ausnahme.vBearbeiten();
        }

        const double dAbschnittNachher = Autos->getAbschnittstrecke();

        if (dAbschnittVorher < dAbschnittNachher)
        {
            p_dVirtuelleSchranke = dAbschnittNachher;
        }

        Autos->vZeichnen(*this);
    }

    for (auto &aktion : p_vAktion)
    {
        aktion();
    }
    p_vAktion.clear();
    p_pFahrzeuge.vAktualisieren();
}

void Weg::vAusgeben(std::ostream &out) const
{
    Simulationsobjekt::vAusgeben(out);
    out << std::setw(15) << p_dLaenge;
    out << "(";
    for (const auto &pAuto : p_pFahrzeuge)
    {
        out << pAuto->getName() << " ";
    };
    out << ")";
}

double Weg::getTempolimit() const
{
    return static_cast<double>(p_eTempolimit);
}

std::shared_ptr<Kreuzung> Weg::getZielKreuzung() const
{
    return p_pZielkreuzung.lock();
}

std::shared_ptr<Weg> Weg::getRueckweg() const
{
    return p_pRueckweg.lock();
}

void Weg::setRueckweg(const std::shared_ptr<Weg> &pRueckweg)
{
    p_pRueckweg = pRueckweg;
}

void Weg::vKopf()
{
    std::cout << std::resetiosflags(std::ios::right) << std::setiosflags(std::ios::left)
              << std::setw(5) << "ID"
              << std::setw(15) << "Name"
              << std::setw(15) << "Laenge"
              << std::setw(20) << "Fahrzeuge" << std::endl
              << std::setfill('-') << std::setw(55) << "" << std::endl
              << std::setfill(' ');
}

void Weg::vAnnahme(std::unique_ptr<Fahrzeug> fahrzeug)
{
    if (!fahrzeug)
    {
        return;
    }
    else
        fahrzeug->vNeueStrecke(*this);
    p_pFahrzeuge.push_back(std::move(fahrzeug));
}

void Weg::vAnnahme(std::unique_ptr<Fahrzeug> fahrzeug, double dStartZeitpunkt)
{
    if (!fahrzeug)
    {
        return;
    }
    else
        fahrzeug->vNeueStrecke(*this, dStartZeitpunkt);
    p_pFahrzeuge.push_front(std::move(fahrzeug));
}

void Weg::vFahrzeugeAusgeben() const
{
    Fahrzeug::vKopf();
    for (const auto &Fahrzeuge : p_pFahrzeuge)
    {
        std::cout << *Fahrzeuge << "\n"
                  << std::endl;
    }
}

double Weg::getLaenge() const
{
    return p_dLaenge;
}

std::unique_ptr<Fahrzeug> Weg::pAbgabe(const Fahrzeug &fahrzeug)
{
    for (auto it = p_pFahrzeuge.begin(); it != p_pFahrzeuge.end(); ++it)
    {
        if (*it && **it == fahrzeug)
        {
            auto fahrzeugAbgegeben = std::move(*it);
            p_pFahrzeuge.erase(it);
            return fahrzeugAbgegeben;
        }
    }
    return nullptr;
}

double Weg::getVirtuelleSchranke() const
{
    if (p_bUeberholverbot)
    {
        return p_dVirtuelleSchranke;
    }
    return p_dLaenge;
}
