// Board.cpp
#include "board.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

Board::Board() {}

Couleur stringToColor(const string& c) {
    if (c == "red") return Couleur::Rouge;
    if (c == "blue") return Couleur::Bleu;
    if (c == "green") return Couleur::Vert;
    if (c == "yellow") return Couleur::Jaune;
    if (c == "black") return Couleur::Noir;
    if (c == "white") return Couleur::Blanc;
    if (c == "orange") return Couleur::Orange;
    return Couleur::Multicolore;
}

void Board::loadRoutesFromCSV(const string& filename) {
    ifstream file(filename);
    string line, word;
    if (!file.is_open()) return;
    
    getline(file, line); 
    while (getline(file, line)) {
        if(line.empty()) continue;
        stringstream ss(line);
        Route r;
        string colorStr, lengthStr;
        
        getline(ss, r.cityA, ',');
        getline(ss, r.cityB, ',');
        getline(ss, colorStr, ',');
        getline(ss, lengthStr, ',');
        
        r.couleur = stringToColor(colorStr);
        r.longueur = stoi(lengthStr);
        r.ownerId = -1;
        routes.push_back(r);
    }
}

void Board::loadTicketsFromCSV(const string& filename) {
    ifstream file(filename);
    string line;
    if (!file.is_open()) return;
    
    getline(file, line); 
    while (getline(file, line)) {
        stringstream ss(line);
        Ticket t;
        string idStr;
        getline(ss, idStr, ',');
        getline(ss, t.cityA, ',');
        getline(ss, t.cityB, ',');
        t.id = stoi(idStr);
        allTickets.push_back(t);
    }
}

bool Board::claimRoute(const string& cityA, const string& cityB, int playerId) {
    for (auto& r : routes) {
        if (((r.cityA == cityA && r.cityB == cityB) || (r.cityA == cityB && r.cityB == cityA)) 
            && r.ownerId == -1) {
            r.ownerId = playerId;
            return true;
        }
    }
    return false; 
}