#ifndef KREUZUNG_H
#define KREUZUNG_H
#include "Simulationsobjekt.h"
#include "vertagt_liste.h"
#include "Weg.h"
#include <memory>

class Kreuzung : public Simulationsobjekt
{
public:
    Kreuzung();
    static void vVerbinde();

protected:
    vertagt::VListe<std::unique_ptr<Weg>> p_pWeg;
    double p_dTankstelle;
};

#endif