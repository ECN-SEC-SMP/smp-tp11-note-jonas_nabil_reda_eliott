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
    std::cout << "=== Les Aventuriers du Rail ===" << std::endl;

    // Initialisation du plateau
    Board gameBoard;
  
    gameBoard.loadRoutesFromCSV("map.csv");
    gameBoard.loadTicketsFromCSV("ticket.csv");
    gameBoard.initDeck();
    gameBoard.initTicketDeck();

    // Création des joueurs
    int nbJoueurs;
    std::cout << "Nombre de joueurs (2-4) : ";
    std::cin >> nbJoueurs;

    std::vector<Joueur> players;
    for (int i = 0; i < nbJoueurs; i++) {
        std::string nom;
        std::cout << "Nom du joueur " << i+1 << " : ";
        std::cin >> nom;
        players.push_back(Joueur(nom));
    }

    // Distribution initiale : 4 cartes + 2 tickets par joueur
    for (Joueur& j : players) {
        for (int i = 0; i < 4; i++) {
            CarteTrain* c = gameBoard.piocherCarte();
            if (c) j.addCarte(c);
        }
        for (int i = 0; i < 2; i++) {
            if (!gameBoard.ticketDeckVide())
                j.addTicket(gameBoard.piocherTicket());
        }
    }

    std::cout << "\nPartie lancee ! Bonne chance !\n" << std::endl;


bool gameOver = false;
int currentPlayerIdx = 0;

while (!gameOver) {
    Joueur& current = players[currentPlayerIdx];
    
    // Affichage état de la partie
    std::cout << "\n==============================" << std::endl;
    std::cout << "Tour de : " << current.getNom() << std::endl;
    std::cout << "Wagons restants : " << current.getNbWagons() << std::endl;
    std::cout << "Score : " << current.getScore() << std::endl;
    std::cout << "Tickets reussis : " << current.getNbTicketsReussis() << std::endl;
    std::cout << "Cartes en main : " << current.getCartes().size() << std::endl;
    std::cout << "Tickets en main : " << current.getTickets().size() << std::endl;
    
    // Choix de l'action
    std::cout << "\nActions : " << std::endl;
    std::cout << "1. Piocher 2 cartes Train" << std::endl;
    std::cout << "2. Prendre une route" << std::endl;
    std::cout << "3. Passer son tour (changer ses tickets)" << std::endl;
    std::cout << "Votre choix : ";
    
    int choix;
    std::cin >> choix;
    
    if (choix == 1) {
        // Piocher 2 cartes
        for (int i = 0; i < 2; i++) {
            CarteTrain* c = gameBoard.piocherCarte();
            if (c) {
                current.addCarte(c);
                std::cout << "Carte piochee !" << std::endl;
            } else {
                std::cout << "Plus de cartes dans la pioche !" << std::endl;
            }
        }
    } 
else if (choix == 2) {
    // Afficher uniquement les routes que le joueur peut prendre
    std::cout << "\nRoutes que vous pouvez prendre :" << std::endl;
    std::vector<int> routesDispo;
    int idx = 0;
    for (auto& r : gameBoard.getRoutes()) {
        if (r.ownerId == -1 && current.peutPrendreVoie(r.longueur, r.couleur)) {
            std::cout << routesDispo.size() << ". " << r.cityA << " -> " << r.cityB 
                      << " (longueur: " << r.longueur << ")" << std::endl;
            routesDispo.push_back(idx);
        }
        idx++;
    }

    std::cout << "Vos cartes : ";
    for (auto& c : current.getCartes()) {
        std::cout << (int)c->getCarteTrain() << " ";
    }
    std::cout << std::endl;

    if (routesDispo.empty()) {
        std::cout << "Aucune route disponible avec vos cartes !" << std::endl;
    } else {
        std::cout << "Numero de la route : ";
        int numRoute;
        std::cin >> numRoute;

        if (numRoute >= 0 && numRoute < (int)routesDispo.size()) {
            auto& r = gameBoard.getRoutes()[routesDispo[numRoute]];
            current.jouerCartes(r.longueur, r.couleur);
            gameBoard.claimRoute(r.cityA, r.cityB, currentPlayerIdx);
            std::cout << "Route prise !" << std::endl;

            // Vérification tickets
            for (Ticket& t : current.getTickets()) {
                if (!t.isCompleted && checkTicket(t, currentPlayerIdx, gameBoard.getRoutes())) {
                    t.isCompleted = true;
                    current.incrementerTicketsReussis();
                    std::cout << "TICKET ! " << t.cityA << " -> " << t.cityB << " reussi !" << std::endl;
                }
            }
        } else {
            std::cout << "Numero invalide !" << std::endl;
        }
    }
}
    else if (choix == 3) {
        // Passer son tour : changer 2 tickets
        for (int i = 0; i < 2; i++) {
            if (!gameBoard.ticketDeckVide()) {
                current.addTicket(gameBoard.piocherTicket());
                std::cout << "Nouveau ticket pioche !" << std::endl;
            }
        }
    }

    // Détection fin de partie
    if (current.getNbTicketsReussis() >= 6 || current.aMoinsDeWagons(0)) {
        std::cout << "\n=== FIN DE PARTIE ===" << std::endl;
        std::cout << current.getNom() << " a declenche la fin !" << std::endl;
        gameOver = true;
    } else {
        currentPlayerIdx = (currentPlayerIdx + 1) % players.size();
    }
}

// Affichage scores finaux
std::cout << "\n=== SCORES FINAUX ===" << std::endl;
for (Joueur& j : players) {
    std::cout << j.getNom() << " : " << j.getNbTicketsReussis() << " tickets reussis" << std::endl;
}

    return 0;
}