// Board.cpp
#include "board.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>   
#include <ctime>     
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


std::vector<Route>& Board::getRoutes() {
    return routes;
}

std::vector<Ticket>& Board::getAllTickets() {
    return allTickets;
}

void Board::initDeck() {
    // 10 cartes par couleur
    std::vector<Couleur> couleurs = {
        Couleur::Rouge, Couleur::Bleu, Couleur::Vert,
        Couleur::Jaune, Couleur::Noir, Couleur::Blanc, Couleur::Orange
    };
    for (Couleur c : couleurs) {
        for (int i = 0; i < 10; i++) {
            deck.push_back(new CarteTrain(c));
        }
    }
    // 12 locomotives
    for (int i = 0; i < 12; i++) {
        deck.push_back(new CarteTrain(Couleur::Multicolore));
    }

    // Mélanger le deck
    srand(time(nullptr));
    for (int i = deck.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        std::swap(deck[i], deck[j]);
    }
}

CarteTrain* Board::piocherCarte() {
    if (deck.empty()) return nullptr;
    CarteTrain* carte = deck.back();
    deck.pop_back();
    return carte;
}


void Board::initTicketDeck() {
    // Copie tous les tickets dans le deck
    ticketDeck = allTickets;

    // Mélanger
    for (int i = ticketDeck.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        std::swap(ticketDeck[i], ticketDeck[j]);
    }
}

Ticket Board::piocherTicket() {
    Ticket t = ticketDeck.back();
    ticketDeck.pop_back();
    return t;
}

bool Board::ticketDeckVide() const {
    return ticketDeck.empty();
}