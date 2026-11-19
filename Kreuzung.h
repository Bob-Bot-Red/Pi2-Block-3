#ifndef KREUZUNG_H
#define KREUZUNG_H
#include "Simulationsobjekt.h"
#include "vertagt_liste.h"
#include "Weg.h"
#include <memory>
#include <list>
#include <stdexcept>

class Kreuzung : public Simulationsobjekt
{
public:
    Kreuzung(const std::string &sName, double dTankstelle = 0.0);
    ~Kreuzung() override;
    static void vVerbinde(
        const std::string &sNameHin,
        const std::string &sNameRueck,
        double dLaenge,
        const std::shared_ptr<Kreuzung> &pStart,
        const std::shared_ptr<Kreuzung> &pZiel,
        Tempolimit eTempolimit = Tempolimit::Autobahn,
        bool bUeberholverbot = true);
        std::shared_ptr<Weg> pZufaelligerWeg(Weg &ankuftsWeg) const;

        void vTanken(Fahrzeug &fahrzeug);
        void vAnnahme(std::unique_ptr<Fahrzeug> fahrzeug, double dStartzeit);
        void vSimulieren() override;

        double getTankstelle() const;

        void vEinlesen(std::istream &in) override;




protected:
    std::list<std::shared_ptr<Weg>> p_pWeg;
    double p_dTankstelle = 0;
};

#endif