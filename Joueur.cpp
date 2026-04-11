#include "Joueur.hpp"
#include <map>
#include <iostream>

Joueur::Joueur(int id, const std::string& n, const std::string& couleur)
    : id(id), nom(n), couleurWagon(couleur), nbwagon(20), aGrandeTraversee(false) {}

int         Joueur::getId()           const { return id; }
std::string Joueur::getNom()          const { return nom; }
std::string Joueur::getCouleurWagon() const { return couleurWagon; }
int         Joueur::getNbWagons()     const { return nbwagon; }
bool        Joueur::hasGrandeTraversee() const { return aGrandeTraversee; }

// +1 si on a la grande traversée
int Joueur::getNbTicketsReussis() const {
    return (int)ticketsReussis.size() + (aGrandeTraversee ? 1 : 0);
}

const std::vector<CarteTrain*>& Joueur::getCartes()         const { return mainCartes; }
const std::vector<Ticket>&      Joueur::getTicketsEnMain()  const { return ticketsEnMain; }
const std::vector<Ticket>&      Joueur::getTicketsReussis() const { return ticketsReussis; }

void Joueur::addCarte(CarteTrain* c) { mainCartes.push_back(c); }

void Joueur::use_wagons(int nb) {
    if (nbwagon >= nb) nbwagon -= nb;
}

bool Joueur::peutPrendreVoie(int longueur, Couleur couleur) const {
    if (nbwagon < longueur) return false;
    int nb = 0, loco = 0;
    for (auto* c : mainCartes) {
        if (c->getCarteTrain() == Couleur::Multicolore) loco++;
        else if (c->getCarteTrain() == couleur) nb++;
    }
    return (nb + loco) >= longueur;
}

std::vector<CarteTrain*> Joueur::jouerCartes(int longueur, Couleur couleur) {
    std::vector<CarteTrain*> defausse;
    if (!peutPrendreVoie(longueur, couleur)) return defausse;
    int aPoser = longueur;
    // d'abord les cartes de la bonne couleur
    for (auto it = mainCartes.begin(); it != mainCartes.end() && aPoser > 0; ) {
        if ((*it)->getCarteTrain() == couleur) {
            defausse.push_back(*it);
            it = mainCartes.erase(it);
            aPoser--;
        } else ++it;
    }
    // puis les locos si besoin
    for (auto it = mainCartes.begin(); it != mainCartes.end() && aPoser > 0; ) {
        if ((*it)->getCarteTrain() == Couleur::Multicolore) {
            defausse.push_back(*it);
            it = mainCartes.erase(it);
            aPoser--;
        } else ++it;
    }
    use_wagons(longueur);
    return defausse;
}

void Joueur::ajouterTicket(const Ticket& t) { ticketsEnMain.push_back(t); }

bool Joueur::completerTicket(int ticketId) {
    for (auto it = ticketsEnMain.begin(); it != ticketsEnMain.end(); ++it) {
        if (it->id == ticketId) {
            it->isCompleted = true;
            ticketsReussis.push_back(*it);
            ticketsEnMain.erase(it);
            return true;
        }
    }
    return false;
}

std::vector<Ticket> Joueur::defausserTousTickets() {
    std::vector<Ticket> tmp = ticketsEnMain;
    ticketsEnMain.clear();
    return tmp;
}

void Joueur::setGrandeTraversee(bool val) { aGrandeTraversee = val; }

bool Joueur::aMoinsDeWagons(int seuil) const { return nbwagon <= seuil; }

void Joueur::afficherEtat() const {
    std::cout << "=== " << nom << " [" << couleurWagon << "] ===\n";
    std::cout << "  Wagons: " << nbwagon << "/20 | Tickets reussis: " << getNbTicketsReussis() << "\n";
    // compter les cartes par couleur
    std::map<Couleur, int> compte;
    for (auto* c : mainCartes) compte[c->getCarteTrain()]++;
    std::cout << "  Main: ";
    for (auto& [col, cnt] : compte)
        std::cout << couleurToString(col) << "x" << cnt << " ";
    std::cout << "\n";
    if (!ticketsEnMain.empty()) {
        std::cout << "  Tickets en cours:\n";
        for (auto& t : ticketsEnMain)
            std::cout << "    [ ] " << t.cityA << " <-> " << t.cityB << "\n";
    }
    if (!ticketsReussis.empty()) {
        std::cout << "  Tickets reussis:\n";
        for (auto& t : ticketsReussis)
            std::cout << "    [OK] " << t.cityA << " <-> " << t.cityB << "\n";
    }
    if (aGrandeTraversee)
        std::cout << "  [BONUS] Grande Traversee!\n";
}