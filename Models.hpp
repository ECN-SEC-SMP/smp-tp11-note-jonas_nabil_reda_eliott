#pragma once 

#include <string>

//couleurs des wagons et des cartes
enum class Couleur { Rouge, Bleu, Vert, Jaune, Noir, Blanc, Orange, Multicolore };

// Structure représentant un Ticket
struct Ticket {
    int id;
    std::string cityA;
    std::string cityB;
    bool isCompleted = false;
};

// Structure d'une route sur le plateau
struct Route {
    std::string cityA;
    std::string cityB;
    Couleur couleur;
    int longueur;
    int ownerId = -1; // -1 = libre, sinon ID du joueur
};
