#include "Joueur.hpp"




//Constructeur
Joueur :: Joueur(std :: string n)   {
    nom = n;

}
Joueur :: Joueur(std :: string n, std :: vector<CarteTrain*> m_carte,  std::vector<ticket*> m_ticket)   {
    nom = n;
    Main_carte = m_carte;
    Main_ticket = m_ticket;

}


    std::string Joueur :: getNom() const  {
        return nom;

    }
    int Joueur :: get_nbwagons()const {

        return nbwagon;
    }

    int Joueur :: get_Nbticketsreussis()const  {

        return nbticketsreussis;
    }
