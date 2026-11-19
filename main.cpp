#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>
#include "Simulationsobjekt.h"
#include "Weg.h"
#include "Fahrzeug.h"
#include "PKW.h"
#include "Fahrrad.h"
#include "Fahrausnahme.h"
#include "SimuClient.h"
#include "SimuClientSocket.h"
#include "vertagt_aktion.h"
#include "vertagt_liste.h"
#include "Kreuzung.h"

using namespace std;
double dEpsilon = 0.001;
double dGlobaleZeit = 0.0;

void vAufgabe_AB1()
{

    int l = 0; // Laufindex für gezielte AUsgabe
    vector<int> ausgabe{15};
    double dTakt = 0.3;

    std::vector<unique_ptr<Fahrzeug>> vecFahrzeuge;
    vecFahrzeuge.push_back(make_unique<PKW>("Audi", 231, 11.4));
    vecFahrzeuge.push_back(make_unique<Fahrrad>("BMX", 19.8));
    for (dGlobaleZeit = 0; dGlobaleZeit < 10; dGlobaleZeit += dTakt)
    {
        auto itL = find(ausgabe.begin(), ausgabe.end(), l);
        if (itL != ausgabe.end())
        {
            std::cout << std::endl
                      << l << " Globalezeit = " << dGlobaleZeit << std::endl;
            Fahrzeug::vKopf();
        }

        for (int i = 0; i < vecFahrzeuge.size(); i++)
        {
            vecFahrzeuge[i]->vSimulieren();
            if (fabs(dGlobaleZeit - 3.0) < dTakt / 2)
            {
                vecFahrzeuge[i]->dTanken();
            }
            if (itL != ausgabe.end())
            {
                std::cout << *vecFahrzeuge[i] << endl;
            }
        }
        l++;
    }
    char c;
    std::cin >> c;
}

void vAufgabe_1()
{
    Fahrzeug statishes("Porsche");
    Fahrzeug *p_dynamisches = new Fahrzeug("Audi");
    std::unique_ptr<Fahrzeug> ptr1 = std::make_unique<Fahrzeug>("a");
    std::unique_ptr<Fahrzeug> ptr2 = std::make_unique<Fahrzeug>("b");
    std::shared_ptr<Fahrzeug> ptr3 = std::make_shared<Fahrzeug>("c");
    std::shared_ptr<Fahrzeug> ptr4 = std::make_shared<Fahrzeug>("d");
    std::cout << "Vorher ptr3" << ptr3.use_count() << std::endl;

    std::shared_ptr<Fahrzeug> ptr5 = ptr3;

    std::cout << "Nachher ptr3" << ptr3.use_count() << std::endl;
    // std::shared_ptr<Fahrzeug> ptr6 = ptr1;
    // Dies klappt nicht da nur einen Besitzer möglich man braucht sowas wie move

    ptr4 = std::move(ptr2);

    std::vector<std::shared_ptr<Fahrzeug>> v_ptr1 = {ptr3, ptr4};
    // Ich kann hiert nur shared ptr Objekte Speichern.
    v_ptr1[0].use_count();
    v_ptr1[1].use_count();

    Fahrzeug::vKopf();
    for (const auto &pFahrzeug : v_ptr1)
    {
        std::cout << *pFahrzeug << std::endl;
        std::cout << std::endl;
    }
    v_ptr1.clear();

    delete p_dynamisches;
}

void vAufgabe_1a()
{
    std::vector<std::unique_ptr<Fahrzeug>> vFahrzeug;

    for (int i = 0; i < 3; i++)
    {
        std::string sAutoName;
        double dAutoSpeed;
        std::cout << "Bitte Auto Name " << i + 1 << " eingeben: ";
        std::cin >> sAutoName;
        std::cout << "Bitte MaxGeschwi. eingeben: ";
        std::cin >> dAutoSpeed;
        auto pFahrzeug = std::make_unique<Fahrzeug>(sAutoName, dAutoSpeed);
        vFahrzeug.push_back(std::move(pFahrzeug));
    }

    double dZeittakt = 0.5;
    for (dGlobaleZeit = dZeittakt; dGlobaleZeit <= 3.0; dGlobaleZeit += dZeittakt)
    {
        std::cout << "Globale Teit:" << dGlobaleZeit << std::endl
                  << std::endl;
        Fahrzeug::vKopf();
        for (const auto &pFahrzeug : vFahrzeug)
        {
            pFahrzeug->vSimulieren();
            std::cout << *pFahrzeug << std::endl;
        }
        std::cout << std::endl;
    }
}

void vAufgabe_2()
{
    std::vector<std::unique_ptr<Fahrzeug>> vFahrzeuge;

    int iAnzahlPKW = 0;
    int iAnzahlFahrrad = 0;

    std::cout << "Wie viele PKWs sollen erzeugt? ";
    std::cin >> iAnzahlPKW;
    for (int i = 0; i < iAnzahlPKW; i++)
    {
        std::string sName;
        double dSpeed, dVerbrauch, dTankvolumen;
        std::cout << "PKW " << i + 1 << " Name, MaxGeschwindigkeit, Verbrauch, Tankvolumen: ";
        std::cin >> sName >> dSpeed >> dVerbrauch >> dTankvolumen;
        vFahrzeuge.push_back(std::make_unique<PKW>(sName, dSpeed, dVerbrauch, dTankvolumen));
    }
    std::cout << "Wie viele Fahrraeder sollen erzeugt?  ";
    std::cin >> iAnzahlFahrrad;
    for (int i = 0; i < iAnzahlFahrrad; i++)
    {
        std::string sName;
        double dSpeed;

        std::cout << "Fahrrad " << i + 1 << " Name, MaxGeschwindigkei:  ";
        std::cin >> sName >> dSpeed;
        vFahrzeuge.push_back(std::make_unique<Fahrrad>(sName, dSpeed));
    }

    double dZeittakt = 0.5;
    // double dEpsilon = 1e-6;

    for (dGlobaleZeit = dZeittakt; dGlobaleZeit <= 5.0 + dEpsilon; dGlobaleZeit += dZeittakt)
    {
        std::cout << "\nGlobale Zeit: " << dGlobaleZeit << std::endl;

        if (std::fabs(dGlobaleZeit - 3.0) < 1e-6)
        {
            std::cout << "--- 3.0 Stunden erreicht: PKWs tanken! ---";
            for (const auto &pFahrzeug : vFahrzeuge)
            {
                pFahrzeug->dTanken();
            };
            std::cout << std::endl;
        }

        Fahrzeug::vKopf();
        for (const auto &pFahrzeuge : vFahrzeuge)
        {
            pFahrzeuge->vSimulieren();
            std::cout << *pFahrzeuge << std::endl;
            std::cout << std::endl;
        }
    }
}

void vAufgabe_3()
{
    std::cout << "Test der Operatoren Auf. 3" << std::endl;
    PKW pkw1("Mercedesm", 200.0, 10.0, 60.0);
    Fahrrad fahrrad1("BMX", 25.0);

    Fahrzeug::vKopf();
    std::cout << pkw1 << std::endl;
    std::cout << fahrrad1 << std::endl;

    std::cout << "\n 1 stunde\n";
    dGlobaleZeit += 1.0;
    pkw1.vSimulieren();
    fahrrad1.vSimulieren();

    Fahrzeug::vKopf();
    std::cout << pkw1 << std::endl;
    std::cout << fahrrad1 << std::endl;

    if (fahrrad1 < pkw1)
    {
        std::cout << "Fahrrad fährt lürzere strecke " << std::endl;
    }

    std::cout << "Stammdaten PKW in Kopie: " << std::endl;
    PKW kopie("LeereKopie", 0.0, 0.0);
    kopie = pkw1;
    Fahrzeug::vKopf();
    std::cout << pkw1 << std::endl;
    std::cout << kopie << std::endl;
}

void vAufgabe_4()
{
    Weg erster("Name", 22.0, Tempolimit::Autobahn);
    Weg::vKopf();
    std::cout << erster << '\n';
}

void vAufgabe_5()
{

    {
        Weg Autobahn1("Ernst", 200.0, Tempolimit::Autobahn);

        Autobahn1.vAnnahme(std::make_unique<PKW>("Audi", 80.0, 6.0));
        Autobahn1.vAnnahme(std::make_unique<PKW>("Mercedes", 100.0, 68.0));
        Autobahn1.vAnnahme(std::make_unique<Fahrrad>("lol", 35.0));
        Autobahn1.vAnnahme(std::make_unique<PKW>("Parkend", 100.0, 5.0), 1.5);

        for (int i = 0; i < 3; i++)
        {
            dGlobaleZeit = i + 0.5;
            try
            {
                Autobahn1.vSimulieren();
            }
            catch (Fahrausnahme &ausnahme)
            {
                ausnahme.vBearbeiten();
            }
            std::cout << "\n Globale" << dGlobaleZeit << std::endl;
            std::cout << Autobahn1 << "\n"
                      << std::endl;
            Autobahn1.vFahrzeugeAusgeben();
        }
    }
}

/*
void vAufgabe_6()
{
    Weg Autobahn1("Autobahn1", 500.0, Tempolimit::Autobahn);
    Weg Autobahn2("Autobahn2", 500.0, Tempolimit::Autobahn);
    Autobahn1.vAnnahme(std::make_unique<PKW>("Audi", 200.0, 2.0));
    Autobahn2.vAnnahme(std::make_unique<PKW>("Mercedea", 120.0, 6.0));

    int koordinaten[] = {700, 250, 100, 250};

    bInitialisiereGrafik(800, 500);
    bZeichneStrasse("Autobahn1", "Autobahn2", 100, 2, koordinaten);

    double dZeitschritte = 0.3;
    for (double dZeit = 0.0; dZeit <= 4.5 + dEpsilon; dZeit += dZeitschritte)
    {
        dGlobaleZeit = dZeit;
        vSetzeZeit(dGlobaleZeit);
        Weg::vKopf();

        //simulieren + zeichnen
        Autobahn1.vSimulieren();
        Autobahn2.vSimulieren();

        //terminal
        std::cout << Autobahn1 << "\n" << Autobahn2 << std::endl;
        Autobahn1.vFahrzeugeAusgeben();
        Autobahn2.vFahrzeugeAusgeben();
        vSleep(100);
    }
    vBeendeGrafik();
} */

void vAufgabe_6()
{
    for (double dzeitschritt : {0.25, 0.3})
    {
        dGlobaleZeit = 0.0;
        if (!bInitialisiereGrafik(800, 500))
        {
            std::cerr << "Grafik kapput \n";
            return;
        }

        int koordinaten[] = {700, 250, 100, 250};

        if (!bZeichneStrasse("Stadt", "Autobahn", 500, 2, koordinaten))
        {
            std::cerr << "Strasse zeichnen fehlgeschlagen \n";
            vBeendeGrafik();
            return;
        }

        Weg stadt("Stadt", 500.0, Tempolimit::Innerorts);
        Weg autobahn("Autobahn", 500.0, Tempolimit::Autobahn, true);

        // Sofort fahrende Fahrzeuge
        stadt.vAnnahme(
            std::make_unique<PKW>("Audi", 120.0, 2.0, 100.0));

        autobahn.vAnnahme(
            std::make_unique<PKW>("BMW", 120.0, 2.0, 100.0));

        // Zunächst parkende Fahrzeuge
        stadt.vAnnahme(
            std::make_unique<PKW>("VW", 100.0, 2.0, 100.0),
            3.0);

        autobahn.vAnnahme(
            std::make_unique<PKW>("Opel", 100.0, 2.0, 100.0),
            3.0);

        stadt.vAnnahme(std::make_unique<Fahrrad>("BMX", 40.0));

        std::cout << "\nZeitschritt: " << dzeitschritt;

        for (int schritt = 0; schritt * dzeitschritt <= 14.0 + dEpsilon; ++schritt)
        {
            dGlobaleZeit = schritt * dzeitschritt;
            vSetzeZeit(dGlobaleZeit);

            stadt.vSimulieren();
            autobahn.vSimulieren();

            std::cout << "\nZeit: " << dGlobaleZeit << "h\n";

            Weg::vKopf();
            std::cout << stadt << "\n"
                      << autobahn << "\n";
            stadt.vFahrzeugeAusgeben();
            autobahn.vFahrzeugeAusgeben();

            vSleep(100);
        }
        vBeendeGrafik();
    }
}

void vAufgabe_6a()
{
    vertagt::VListe<int> zahlen;
    constexpr unsigned int seed =
        (__DATE__[9] - '0') * 10 + (__DATE__[10] - '0');
    static std::mt19937 device(seed);
    std::uniform_int_distribution<int> dist(1, 10);
    for (int i = 0; i < 10; ++i)
    {
        int zuf = dist(device);
        zahlen.push_back(zuf);
    }

    zahlen.vAktualisieren();

    std::cout << "1. Ursprungliche: \n";
    for (int zahl : zahlen)
    {
        std::cout << zahl << ' ';
    }
    std::cout << "\n";

    for (auto it = zahlen.begin(); it != zahlen.end(); ++it)
    {
        if (*it > 5)
        {
            zahlen.erase(it);
        }
    }

    std::cout << "2. nach erase vor aktual. \n";
    for (int zahl : zahlen)
    {
        std::cout << zahl << ' ';
    }
    std::cout << "\n";

    zahlen.vAktualisieren();

    std::cout << "3. nach aktualisieren\n";
    for (int zahl : zahlen)
    {
        std::cout << zahl << ' ';
    }
    std::cout << "\n";

    zahlen.push_front(1);
    zahlen.push_back(10);
    zahlen.vAktualisieren();

    std::cout << "4. mit 1. am Anfang 10 am Ende\n";
    for (int zahl : zahlen)
    {
        std::cout << zahl << ' ';
    }
    std::cout << "\n";
}


void vAufgabe_7()
{
    dGlobaleZeit = 0.0;

    if (!bInitialisiereGrafik(1100, 700))
    {
        std::cerr << "Grafik konnte nicht launchen.\n";
        return;
    }

    auto pKr1 = std::make_shared<Kreuzung>("Kr1");
    auto pKr2 = std::make_shared<Kreuzung>("Kr2", 1000.0);
    auto pKr3 = std::make_shared<Kreuzung>("Kr3");
    auto pKr4 = std::make_shared<Kreuzung>("Kr4");

    Kreuzung::vVerbinde("W12", "W21", 40, pKr1, pKr2, Tempolimit::Innerorts, true);
    Kreuzung::vVerbinde("W23a", "W32a", 115, pKr2, pKr3, Tempolimit::Autobahn, false);
    Kreuzung::vVerbinde("W23b", "W32b", 40, pKr2, pKr3, Tempolimit::Innerorts, true);
    Kreuzung::vVerbinde("W24", "W42", 55, pKr2, pKr4, Tempolimit::Innerorts, true);
    Kreuzung::vVerbinde("W34", "W43", 85, pKr3, pKr4, Tempolimit::Autobahn, false);
    Kreuzung::vVerbinde("W44a", "W44b", 130, pKr4, pKr4, Tempolimit::Landstrasse, false);

    int strasse1[] = {680, 40, 680, 300};
    int strasse2[] = {680, 300, 850, 300, 970, 390, 970, 500, 850, 570, 680, 570};
    int strasse3[] = {680, 300, 680, 570};
    int strasse4[] = {680, 300, 320, 300};
    int strasse5[] = {680, 570, 500, 570, 350, 510, 320, 420, 320, 300};
    int strasse6[] = {320, 300, 320, 150, 200, 60, 80, 90, 70, 250, 170, 300, 320, 300};

    const bool bStrasseGezeichnet =
        bZeichneStrasse("W12", "W21", 40, 2, strasse1) &&
        bZeichneStrasse("W23a", "W32a", 115, 6, strasse2) &&
        bZeichneStrasse("W23b", "W32b", 40, 2, strasse3) &&
        bZeichneStrasse("W24", "W42", 55, 2, strasse4) &&
        bZeichneStrasse("W34", "W43", 85, 5, strasse5) &&
        bZeichneStrasse("W44a", "W44b", 130, 7, strasse6);

    if (!bStrasseGezeichnet)
    {
        std::cerr << "Mindest eine Sraße nicht gezeichnet";
        vBeendeGrafik();
        return;
    }

    bZeichneKreuzung(680, 40);
    bZeichneKreuzung(680, 300);
    bZeichneKreuzung(680, 570);
    bZeichneKreuzung(320, 300);

    pKr1->vAnnahme(std::make_unique<PKW>("Audi", 120.0, 8.0), 0.0);
    pKr1->vAnnahme(std::make_unique<PKW>("BMW", 100.0, 7.0), 1.0);
    pKr1->vAnnahme(std::make_unique<Fahrrad>("BMX", 25.0), 2.0);

    constexpr double dZeitschritte = 0.25;
    constexpr int iAnzahlSchritte = 80;

    for (int i = 0; i <= iAnzahlSchritte; ++i)
    {
        dGlobaleZeit = i * dZeitschritte;
        vSetzeZeit(dGlobaleZeit);
        pKr1->vSimulieren();
        pKr2->vSimulieren();
        pKr3->vSimulieren();
        pKr4->vSimulieren();
        vSleep(100);
    }
    vBeendeGrafik();
}
int main()
{
    /*
    vAufgabe_1();
    vAufgabe_1a();
    vAufgabe_AB1();
    vAufgabe_2();
    vAufgabe_4();
    vAufgabe_3();
    vAufgabe_5();
    vAufgabe_6a();
    vAufgabe_6();
    */
    vAufgabe_7();
    return 0;
}