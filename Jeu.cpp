#include "Jeu.hpp"
#include <iostream>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <map>
#include <queue>
#include <set>
#include <limits>
using namespace std;

// côte ouest et côte est pour la grande traversée
const vector<string> Jeu::villesOuest = {"Seattle", "San Francisco", "Los Angeles"};
const vector<string> Jeu::villesEst   = {"New York", "Washington", "Miami"};

Jeu::Jeu() : joueurCourantIdx(0), partieTerminee(false), grandeTraverseeAttribuee(false) {}

Jeu::~Jeu() {
    for (auto* j : joueurs)       delete j;
    for (auto* c : toutesLesCartes) delete c;
}

void Jeu::initialiser(int nbJoueurs) {
    board.loadRoutesFromCSV("csv/map.csv");
    board.loadTicketsFromCSV("csv/ticket.csv");

    vector<string> couleursJ = {"Rouge", "Jaune", "Vert", "Bleu"};
    cout << "\n=== Configuration des joueurs ===\n";
    for (int i = 0; i < nbJoueurs; i++) {
        cout << "Nom du joueur " << (i + 1) << " (" << couleursJ[i] << ") : ";
        string nom;
        cin >> nom;
        joueurs.push_back(new Joueur(i, nom, couleursJ[i]));
    }

    creerPiocheCartes();

    for (auto& t : board.getAllTickets())
        piocheTickets.push_back(t);
    auto rng = default_random_engine{random_device{}()};
    shuffle(piocheTickets.begin(), piocheTickets.end(), rng);

    distribuerDepart();
}

void Jeu::creerPiocheCartes() {
    // 10 cartes par couleur (6 couleurs + orange) + 12 locos
    vector<Couleur> couleurs = {
        Couleur::Rouge, Couleur::Bleu,  Couleur::Vert,
        Couleur::Jaune, Couleur::Noir,  Couleur::Blanc, Couleur::Orange
    };
    for (auto col : couleurs)
        for (int i = 0; i < 10; i++) {
            auto* c = new CarteTrain(col);
            toutesLesCartes.push_back(c);
            piocheCartes.push_back(c);
        }
    for (int i = 0; i < 12; i++) {
        auto* c = new CarteTrain(Couleur::Multicolore);
        toutesLesCartes.push_back(c);
        piocheCartes.push_back(c);
    }
    melangerCartes();
}

void Jeu::melangerCartes() {
    auto rng = default_random_engine{random_device{}()};
    shuffle(piocheCartes.begin(), piocheCartes.end(), rng);
}

void Jeu::distribuerDepart() {
    // 4 cartes train et 2 tickets à chaque joueur
    for (auto* j : joueurs) {
        for (int i = 0; i < 4; i++) j->addCarte(piocherCarte());
        for (int i = 0; i < 2; i++) j->ajouterTicket(piocherTicket());
    }
}

CarteTrain* Jeu::piocherCarte() {
    if (piocheCartes.empty()) {
        if (defausseCartes.empty()) { cout << "Plus de cartes!\n"; return nullptr; }
        for (auto* c : defausseCartes) piocheCartes.push_back(c);
        defausseCartes.clear();
        melangerCartes();
    }
    auto* c = piocheCartes.front();
    piocheCartes.pop_front();
    return c;
}

Ticket Jeu::piocherTicket() {
    if (piocheTickets.empty()) {
        if (defausseTickets.empty()) throw runtime_error("Plus de tickets!");
        for (auto& t : defausseTickets) piocheTickets.push_back(t);
        defausseTickets.clear();
        auto rng = default_random_engine{random_device{}()};
        shuffle(piocheTickets.begin(), piocheTickets.end(), rng);
    }
    Ticket t = piocheTickets.front();
    piocheTickets.pop_front();
    return t;
}

void Jeu::lancerPartie() {
    cout << "\n=== La partie commence ! ===\n";

    while (!partieTerminee) {
        Joueur& j = *joueurs[joueurCourantIdx];
        afficherEtat();
        jouerTour(j);
        verifierTickets(j);
        verifierGrandeTraversee(j);

        // fin de partie si 6 tickets réussis ou plus de wagons
        if (j.getNbTicketsReussis() >= 6 || j.aMoinsDeWagons(0)) {
            cout << "\n*** " << j.getNom() << " declenche la fin de partie ! ***\n";
            partieTerminee = true;
        }
        if (!partieTerminee)
            joueurCourantIdx = (joueurCourantIdx + 1) % joueurs.size();
    }

    afficherResultats();
}

void Jeu::jouerTour(Joueur& j) {
    cout << "\n--- Tour de " << j.getNom()
         << " [" << j.getCouleurWagon() << "]"
         << " | Wagons: " << j.getNbWagons()
         << " | Tickets: " << j.getNbTicketsReussis() << "/6 ---\n";
    cout << "Actions:\n";
    cout << "  1. Piocher 2 cartes Train\n";
    cout << "  2. Prendre une voie ferree\n";
    cout << "  3. Passer (defausser tickets, reprendre 2 nouveaux)\n";
    cout << "Choix: ";

    int choix = 0;
    while (!(cin >> choix) || choix < 1 || choix > 3) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Choix invalide (1-3): ";
    }

    switch (choix) {
        case 1: actionPiocherCartes(j);    break;
        case 2: actionPrendreVoie(j);      break;
        case 3: actionEchangerTickets(j);  break;
    }
}

void Jeu::actionPiocherCartes(Joueur& j) {
    for (int i = 0; i < 2; i++) {
        auto* c = piocherCarte();
        if (c) {
            j.addCarte(c);
            cout << "  -> Pioche: " << couleurToString(c->getCarteTrain()) << "\n";
        }
    }
}

void Jeu::actionPrendreVoie(Joueur& j) {
    auto& routes = board.getRoutes();

    // lister les voies libres (sans voie parallèle déjà prise par ce joueur)
    cout << "\nVoies disponibles:\n";
    vector<int> dispo;
    for (int i = 0; i < (int)routes.size(); i++) {
        if (routes[i].ownerId != -1) continue;
        // interdit de prendre les 2 voies parallèles
        bool parallelePrise = false;
        for (auto& r2 : routes) {
            if (r2.ownerId == j.getId() &&
                ((r2.cityA == routes[i].cityA && r2.cityB == routes[i].cityB) ||
                 (r2.cityA == routes[i].cityB && r2.cityB == routes[i].cityA))) {
                parallelePrise = true;
                break;
            }
        }
        if (!parallelePrise) {
            cout << "  " << dispo.size() << ". "
                 << routes[i].cityA << " -> " << routes[i].cityB
                 << " [" << couleurToString(routes[i].couleur)
                 << ", " << routes[i].longueur << " wagons]\n";
            dispo.push_back(i);
        }
    }

    if (dispo.empty()) { cout << "Aucune voie disponible.\n"; return; }

    cout << "Choisir (0-" << (dispo.size() - 1) << ") ou -1 pour annuler: ";
    int choix;
    cin >> choix;
    if (choix < 0 || choix >= (int)dispo.size()) { cout << "Annule.\n"; return; }

    auto& route = routes[dispo[choix]];
    if (!j.peutPrendreVoie(route.longueur, route.couleur)) {
        cout << "Pas assez de cartes " << couleurToString(route.couleur) << ".\n";
        return;
    }

    auto defausse = j.jouerCartes(route.longueur, route.couleur);
    for (auto* c : defausse) defausseCartes.push_back(c);
    route.ownerId = j.getId();
    cout << "-> " << j.getNom() << " prend "
         << route.cityA << " - " << route.cityB << " !\n";
}

void Jeu::actionEchangerTickets(Joueur& j) {
    // défausser les tickets en main et en reprendre 2
    auto def = j.defausserTousTickets();
    for (auto& t : def) defausseTickets.push_back(t);
    cout << "  " << def.size() << " ticket(s) defausse(s).\n";

    int disponibles = (int)piocheTickets.size() + (int)defausseTickets.size();
    int nb = min(2, disponibles);
    for (int i = 0; i < nb; i++) {
        try {
            Ticket t = piocherTicket();
            j.ajouterTicket(t);
            cout << "  -> Nouveau ticket: " << t.cityA << " <-> " << t.cityB << "\n";
        } catch (...) {
            cout << "Plus de tickets disponibles.\n";
            break;
        }
    }
}

void Jeu::verifierTickets(Joueur& j) {
    // copie car on modifie ticketsEnMain pendant l'iteration
    vector<Ticket> enMain = j.getTicketsEnMain();
    for (auto& t : enMain) {
        if (checkChemin(t.cityA, t.cityB, j.getId())) {
            j.completerTicket(t.id);
            cout << "\n[TICKET!] " << j.getNom()
                 << " relie " << t.cityA << " <-> " << t.cityB << " !\n";
            // piocher un nouveau ticket
            try {
                Ticket nouveau = piocherTicket();
                j.ajouterTicket(nouveau);
                cout << "  -> Nouveau ticket: " << nouveau.cityA << " <-> " << nouveau.cityB << "\n";
            } catch (...) {}
        }
    }
}

// BFS pour vérifier si deux villes sont reliées par les routes du joueur
bool Jeu::checkChemin(const string& dep, const string& arr, int pid) {
    map<string, vector<string>> adj;
    for (auto& r : board.getRoutes()) {
        if (r.ownerId == pid) {
            adj[r.cityA].push_back(r.cityB);
            adj[r.cityB].push_back(r.cityA);
        }
    }
    queue<string> q;
    set<string>   visited;
    q.push(dep);
    visited.insert(dep);
    while (!q.empty()) {
        string cur = q.front(); q.pop();
        if (cur == arr) return true;
        for (auto& v : adj[cur])
            if (!visited.count(v)) { visited.insert(v); q.push(v); }
    }
    return false;
}

void Jeu::verifierGrandeTraversee(Joueur& j) {
    if (grandeTraverseeAttribuee) return;
    for (auto& vo : villesOuest)
        for (auto& ve : villesEst)
            if (checkChemin(vo, ve, j.getId())) {
                grandeTraverseeAttribuee = true;
                j.setGrandeTraversee(true);
                cout << "\n[GRANDE TRAVERSEE!] " << j.getNom()
                     << " relie l'ouest a l'est !\n";
                return;
            }
}

void Jeu::afficherEtat() {
    cout << "\n========== ETAT DE LA PARTIE ==========\n";
    cout << "Pioche cartes: " << piocheCartes.size()
         << " | Pioche tickets: " << piocheTickets.size() << "\n";
    board.afficherRoutesPrises();
    for (auto* j : joueurs) j->afficherEtat();
    cout << "========================================\n";
}

void Jeu::afficherResultats() {
    cout << "\n========== FIN DE PARTIE ==========\n";
    int maxTickets = -1;
    Joueur* gagnant = nullptr;
    for (auto* j : joueurs) {
        cout << "  " << j->getNom() << " : " << j->getNbTicketsReussis() << " tickets reussis\n";
        if (j->getNbTicketsReussis() > maxTickets) {
            maxTickets = j->getNbTicketsReussis();
            gagnant    = j;
        }
    }
    if (gagnant)
        cout << "\nVainqueur : " << gagnant->getNom()
             << " avec " << maxTickets << " ticket(s) !\n";
    cout << "====================================\n";
}
