#pragma once 

#include <string>
#include <vector>
#include <iostream>
#include "locomotive.hpp"
#include "carteTrain.hpp"

class Joueur    {

private :

std :: string nom;

int  nbticketsreussis;

int nbwagon;

std :: vector<CarteTrain*> Main_carte;

std :: vector<ticket*> Main_ticket;





public :
    //Constructeur
    Joueur(std :: string n);    
    Joueur(std :: string n, std :: vector<CarteTrain*> m_carte,  std::vector<ticket*> m_ticket);

        // Getters
    std::string getNom() const;
    int get_nbwagons() const;
    int get_Nbticketsreussis() const;

    //Methodes
    void add_newticket();
    void add_newcarte();
    void use_wagons(int nb);
    bool peutPrendreVoie(int longueur, Couleur couleur) const ;

};

