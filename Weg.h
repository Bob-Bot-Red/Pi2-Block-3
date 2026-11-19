#ifndef WEG_H
#define WEG_H

#include "Simulationsobjekt.h"
#include "Fahrzeug.h"
#include "Tempolimit.h"
#include "vertagt_liste.h"
#include <list>
#include <memory>
#include <functional>
#include <utility>

class Kreuzung;

class Fahrzeug;

class Weg : public Simulationsobjekt
{

public:
    Weg();
    Weg(const std::string &sName, double dLaenge, Tempolimit eTempolimit = Tempolimit::Autobahn, bool Ueberholverbot = true, std::shared_ptr<Kreuzung> pZielkreuzung = nullptr);
    virtual ~Weg();

    static void vKopf();
    virtual void vSimulieren() override;
    virtual void vAusgeben(std::ostream &out) const override;
    void vAnnahme(std::unique_ptr<Fahrzeug> fahrzeug);
    void vAnnahme(std::unique_ptr<Fahrzeug> fahrzeug, double dStartZeitpunkt);
    void vFahrzeugeAusgeben() const;
    double getLaenge() const;
    double getTempolimit() const;

    std::shared_ptr<Kreuzung> getZielKreuzung() const;
    std::shared_ptr<Weg> getRueckweg() const;  
    void setRueckweg(const std::shared_ptr<Weg> &pRueckweg);  


    std::unique_ptr<Fahrzeug> pAbgabe(const Fahrzeug &fahrzeug);

    template <typename Aktion>
    void vVerzoegert(Aktion &&aktion)
    {
        p_vAktion.emplace_back(std::forward<Aktion>(aktion));
    }

    double getVirtuelleSchranke() const;

private:
    double p_dLaenge = 0.0;
    Tempolimit p_eTempolimit;
    vertagt::VListe<std::unique_ptr<Fahrzeug>> p_pFahrzeuge;
    std::list<std::function<void()>> p_vAktion;
    bool p_bUeberholverbot = true;
    double p_dVirtuelleSchranke = 0.0;
    std::weak_ptr<Weg> p_pRueckweg;
    const std::weak_ptr<Kreuzung> p_pZielkreuzung;
};

#endif