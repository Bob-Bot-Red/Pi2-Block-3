#ifndef SIMULATIONSOBJEKT_H
#define SIMULATIONSOBJEKT_H

#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>

class Simulationsobjekt
{
public:
    Simulationsobjekt();
    Simulationsobjekt(const std::string &sName);
    virtual ~Simulationsobjekt();

    virtual void vAusgeben(std::ostream &out) const;
    virtual void vSimulieren() = 0;

    int getID() const;
    const std::string &getName() const;
    bool operator==(const Simulationsobjekt &rhs) const;
    Simulationsobjekt(const Simulationsobjekt &) = delete;
    Simulationsobjekt &operator=(const Simulationsobjekt &rhs);

    virtual void vEinlesen(std::istream &in);


    


protected:
    double p_dZeit = 0.0;
    std::string p_sName = "";

private:
    const int p_iID;
    static int p_iMaxID;
};

std::ostream &operator<<(std::ostream &out, const Simulationsobjekt &s);


std::istream &operator>>(std::istream &in, Simulationsobjekt &oobjekt);


#endif

