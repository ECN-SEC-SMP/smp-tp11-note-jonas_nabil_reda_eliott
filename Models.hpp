#pragma once

#include <string>

enum class Couleur {
    Rouge, Bleu, Vert, Jaune, Noir, Blanc, Orange, Violette, Multicolore
};

struct Route {
    std::string cityA;
    std::string cityB;
    Couleur couleur;
    int longueur;
    int ownerId;
};

struct Ticket {
    int id;
    std::string cityA;
    std::string cityB;
    bool complete;
};