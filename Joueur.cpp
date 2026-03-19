#include "Joueur.hpp"




//Constructeur

Joueur :: Joueur(std :: string n)   {
    nom = n;
    nbwagon = 20;
}
Joueur :: Joueur(std :: string n, std :: vector<CarteTrain*> m_carte,  std::vector<ticket*> m_ticket)   {
    nom = n;
    Main_carte = m_carte;
    Main_ticket = m_ticket;


}


// Getters

std::string Joueur :: getNom() const  {
    return nom;

}
int Joueur :: get_nbwagons()const {

    return nbwagon;
}

int Joueur :: get_Nbticketsreussis()const  {

    return nbticketsreussis;
}


//Methodes


void Joueur :: add_newticket()  {
    
    this->Main_ticket.push_back(new ticket);

}


void Joueur :: add_newcarte()   {
    
    this->Main_carte.push_back(new CarteTrain());
}


    void Joueur :: use_wagons(int nb) {
        if (nbwagon) {
            nbwagon--;
        }
    }
