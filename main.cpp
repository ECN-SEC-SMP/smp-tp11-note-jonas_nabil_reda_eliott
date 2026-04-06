// main.cpp
#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include "board.hpp"
#include "Joueur.hpp"

// Fonction cruciale pour 20% de la note : Détection de chemin continu (BFS)
bool checkTicket(const Ticket& ticket, int playerId, const std::vector<Route>& allRoutes) {
    // Créer un graphe d'adjacence uniquement avec les routes possédées par le joueur
    std::map<std::string, std::vector<std::string>> adjList;
    for (const auto& r : allRoutes) {
        if (r.ownerId == playerId) {
            adjList[r.cityA].push_back(r.cityB);
            adjList[r.cityB].push_back(r.cityA);
        }
    }

    // Parcours en largeur (BFS)
    std::queue<std::string> q;
    std::set<std::string> visited;

    q.push(ticket.cityA);
    visited.insert(ticket.cityA);

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();

        if (current == ticket.cityB) return true; // Chemin trouvé !

        for (const std::string& neighbor : adjList[current]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }
    return false;
}

int main() {
    std::cout << "Bienvenue dans Les Aventuriers du Rail - Mon Premier Voyage !" << std::endl;
    
    Board gameBoard;
    gameBoard.loadRoutesFromCSV("map.csv");
    gameBoard.loadTicketsFromCSV("ticket.csv");
    
    std::vector<Joueur> players;
    players.push_back(Joueur("Alice"));
    players.push_back(Joueur("Bob"));
    
    // Boucle de jeu basique (à enrichir par le groupe)
    bool gameOver = false;
    int currentPlayerIdx = 0;
    
    while (!gameOver) {
        Joueur& current = players[currentPlayerIdx];
        std::cout << "C'est au tour de " << current.getNom() << " de jouer." << std::endl;
        std::cout << "Actions : 1. Piocher Train | 2. Prendre Route | 3. Piocher Ticket" << std::endl;
        std::cout << "Choix (simulation auto pour test): 2" << std::endl;

        // Simulation d'une prise de route
        if(currentPlayerIdx == 0) {
            gameBoard.claimRoute("Seattle", "Calgary", currentPlayerIdx);
            current.use_wagons(4);
        }

        // Simulation de vérification de victoire
        if (current.getNbTicketsReussis() >= 6 || current.getNbWagons() <= 2) {
            std::cout << "FIN DE PARTIE ! " << current.getNom() << " a declenche la fin." << std::endl;
            gameOver = true;
        } else {
            // Passer au joueur suivant
            currentPlayerIdx = (currentPlayerIdx + 1) % players.size();

            // Pour eviter une boucle infinie dans ce squelette
            static int turns = 0;
            if(turns++ > 2) gameOver = true;
        }
    }
    
    return 0;
}