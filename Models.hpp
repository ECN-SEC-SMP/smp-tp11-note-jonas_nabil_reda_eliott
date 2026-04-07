#pragma once 

#include <string>

//couleurs des wagons et des cartes
enum class Color { RED, BLUE, GREEN, YELLOW, BLACK, WHITE, ORANGE, LOCOMOTIVE, ANY };

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
    Color color;
    int length;
    int ownerId = -1; // -1 = libre, sinon ID du joueur
};
