#pragma once 

#include <string>

// Couleurs des wagons et des cartes train
enum class Couleur { Rouge, Bleu, Vert, Jaune, Noir, Blanc, Orange, Multicolore };

// Convertit une couleur en chaîne lisible
inline std::string couleurToString(Couleur c) {
    switch (c) {
        case Couleur::Rouge:       return "Rouge";
        case Couleur::Bleu:        return "Bleu";
        case Couleur::Vert:        return "Vert";
        case Couleur::Jaune:       return "Jaune";
        case Couleur::Noir:        return "Noir";
        case Couleur::Blanc:       return "Blanc";
        case Couleur::Orange:      return "Orange";
        case Couleur::Multicolore: return "Locomotive";
        default:                   return "Inconnu";
    }
}

// Ticket reliant deux villes
struct Ticket {
    int id;
    std::string cityA;
    std::string cityB;
    bool isCompleted = false;
};

// Route sur le plateau de jeu
struct Route {
    std::string cityA;
    std::string cityB;
    Couleur couleur;
    int longueur;
    int ownerId = -1; // -1 = libre, sinon ID du joueur
};
