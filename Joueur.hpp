#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "carteTrain.hpp"
#include "Models.hpp"

class Joueur {

private:
    std::string nom;
    int nbwagon;
    int score;
    int nbticketsreussis;
    std::vector<CarteTrain*> Main_carte;
    std::vector<Ticket> mesTickets;

public:
    // Constructeur
    Joueur(std::string n);

    // Getters
    std::string getNom() const;
    int getNbWagons() const;
    int getScore() const;
    int getNbTicketsReussis() const;
    const std::vector<CarteTrain*>& getCartes() const;
    std::vector<Ticket>& getTickets();

    // Methodes
    void addCarte(CarteTrain* carte);
    void addWagons(int nb);
    void use_wagons(int nb);
    bool peutPrendreVoie(int longueur, Couleur couleur) const;
    std::vector<CarteTrain*> jouerCartes(int longueur, Couleur couleur);
    void incrementerScore(int points);
    void incrementerTicketsReussis();
    bool aMoinsDeWagons(int seuil) const;
    void addTicket(Ticket t);

};
