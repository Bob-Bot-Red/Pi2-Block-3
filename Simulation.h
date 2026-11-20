#ifndef SIMULATION_H
#define SIMULATION_H

#pragma once

#include <iosfwd>
#include <map>
#include <memory>
#include <string>
#include <iostream>

class Kreuzung;

class Simulation
{
    public: 
    void vEinlesen(std::istream& in);
    void vSimulieren(double dDauer, double dZeitschritt);

    private:
    std::map<std::string, std::shared_ptr<Kreuzung>> p_pKreuzungen;
    std::shared_ptr<Kreuzung> pKreuzungFinden(const std::string& sName) const;

};

#endif
