#include "Partie.hpp"

#include <algorithm>
#include <iostream>

// ============================================================
//  Construction
// ============================================================

Partie::Partie()
    : rng_(std::random_device{}()) {}

// ============================================================
//  Initialisation
// ============================================================

bool Partie::initialiser(const std::vector<std::string>& nomsJoueurs,
                         const std::string& cheminVoiesCSV,
                         const std::string& cheminTicketsCSV) {

    // Vérification du nombre de joueurs
    if (nomsJoueurs.size() < 2 || nomsJoueurs.size() > 4) {
        std::cerr << "[Erreur] Le nombre de joueurs doit être compris entre 2 et 4.\n";
        return false;
    }

    // Chargement du plateau (voies) via la classe Plateau
    if (!plateau_.chargerVoies(cheminVoiesCSV)) return false;

    // Chargement des tickets
    if (!chargerTickets(cheminTicketsCSV)) return false;

    // Création des joueurs
    const std::vector<std::string> couleursWagon = {"Rouge", "Jaune", "Vert", "Bleu"};
    joueurs_.clear();
    for (size_t i = 0; i < nomsJoueurs.size(); ++i)
        joueurs_.emplace_back(nomsJoueurs[i], couleursWagon[i]);

    // Construction de la pioche de cartes (72 cartes)
    piocheCartes_.clear();
    defausseCartes_.clear();
    const std::vector<Couleur> couleursCartes = {
        Couleur::JAUNE, Couleur::VERTE, Couleur::BLANCHE,
        Couleur::NOIRE, Couleur::BLEUE, Couleur::ROUGE
    };
    for (Couleur c : couleursCartes)
        for (int i = 0; i < 10; ++i)
            piocheCartes_.emplace_back(c);
    for (int i = 0; i < 12; ++i)
        piocheCartes_.emplace_back(Couleur::LOCOMOTIVE);

    melangerPiocheCartes();

    // Distribution : 4 cartes Train + 2 tickets par joueur
    for (int i = 0; i < static_cast<int>(joueurs_.size()); ++i) {
        for (int k = 0; k < 4; ++k) piocherUneCarte(i);
        for (int k = 0; k < 2; ++k) piocherUnTicket(i);
    }

    joueurCourant_       = 0;
    ticketsReussisTotal_ = 0;
    partieEnCours_       = true;

    std::cout << "\n=== Partie initialisée avec " << joueurs_.size() << " joueurs ===\n\n";
    return true;
}

// Chargement des tickets depuis CSV (format : villeA,villeB)
bool Partie::chargerTickets(const std::string& cheminCSV) {
    std::ifstream f(cheminCSV);
    if (!f.is_open()) {
        std::cerr << "[Erreur] Impossible d'ouvrir : " << cheminCSV << "\n";
        return false;
    }
    std::string ligne;
    while (std::getline(f, ligne)) {
        if (ligne.empty() || ligne[0] == '#') continue;
        std::istringstream ss(ligne);
        std::string a, b;
        if (!std::getline(ss, a, ',')) continue;
        if (!std::getline(ss, b, ',')) continue;
        piocheTickets_.emplace_back(a, b);
    }
    melangerPiocheTickets();
    std::cout << "[Info] " << piocheTickets_.size() << " tickets chargés.\n";
    return true;
}

// ============================================================
//  Mélanges / recharges
// ============================================================

void Partie::melangerPiocheCartes() {
    std::shuffle(piocheCartes_.begin(), piocheCartes_.end(), rng_);
}

void Partie::melangerPiocheTickets() {
    std::shuffle(piocheTickets_.begin(), piocheTickets_.end(), rng_);
}

void Partie::rechargerPiocheCartes() {
    if (defausseCartes_.empty()) return;
    piocheCartes_.insert(piocheCartes_.end(),
                         defausseCartes_.begin(), defausseCartes_.end());
    defausseCartes_.clear();
    melangerPiocheCartes();
    std::cout << "[Info] Pioche de cartes rechargée depuis la défausse.\n";
}

void Partie::rechargerPiocheTickets() {
    if (defausseTickets_.empty()) return;
    piocheTickets_.insert(piocheTickets_.end(),
                          defausseTickets_.begin(), defausseTickets_.end());
    defausseTickets_.clear();
    melangerPiocheTickets();
    std::cout << "[Info] Pioche de tickets rechargée depuis la défausse.\n";
}

// ============================================================
//  Pioches unitaires
// ============================================================

bool Partie::piocherUneCarte(int idxJoueur) {
    if (piocheCartes_.empty()) rechargerPiocheCartes();
    if (piocheCartes_.empty()) return false;

    joueurs_[idxJoueur].main.push_back(piocheCartes_.back());
    piocheCartes_.pop_back();
    return true;
}

bool Partie::piocherUnTicket(int idxJoueur) {
    if (piocheTickets_.empty()) rechargerPiocheTickets();
    if (piocheTickets_.empty()) return false;

    joueurs_[idxJoueur].tickets.push_back(piocheTickets_.back());
    piocheTickets_.pop_back();
    return true;
}

// ============================================================
//  Actions d'un tour
// ============================================================

void Partie::piocherCartes(int idxJoueur) {
    for (int i = 0; i < 2; ++i) {
        if (piocherUneCarte(idxJoueur)) {
            const CarteTrain& c = joueurs_[idxJoueur].main.back();
            std::cout << "  " << joueurs_[idxJoueur].nom
                      << " pioche : " << couleurToString(c.couleur) << "\n";
        } else {
            std::cout << "  Plus aucune carte disponible.\n";
        }
    }
}

bool Partie::prendreVoie(int idxJoueur, int idxVoie) {
    Joueur& joueur = joueurs_[idxJoueur];

    // Déléguer la vérification et la prise de voie au Plateau
    if (!plateau_.prendreVoie(idxVoie, idxJoueur, joueur)) return false;

    // Mettre à jour les tickets et la grande traversée
    mettreAJourTickets(idxJoueur);

    if (!joueur.aCarteBonus && plateau_.grandeTraverseeReussie(idxJoueur)) {
        joueur.aCarteBonus = true;
        ++ticketsReussisTotal_;
        std::cout << "  *** " << joueur.nom
                  << " remporte la carte Bonus Grande Traversée ! ***\n";
    }

    return true;
}

void Partie::passerTour(int idxJoueur) {
    Joueur& joueur = joueurs_[idxJoueur];

    // Défausser les deux tickets en cours
    for (auto& t : joueur.tickets)
        defausseTickets_.push_back(t);
    joueur.tickets.clear();

    std::cout << "  " << joueur.nom << " passe son tour et change ses tickets.\n";

    // Piocher deux nouveaux tickets
    for (int i = 0; i < 2; ++i)
        if (!piocherUnTicket(idxJoueur))
            std::cout << "  Plus de tickets disponibles.\n";
}

// ============================================================
//  Vérification des tickets
// ============================================================

void Partie::mettreAJourTickets(int idxJoueur) {
    Joueur& joueur = joueurs_[idxJoueur];

    auto it = joueur.tickets.begin();
    while (it != joueur.tickets.end()) {
        // Déléguer la vérification de connectivité au Plateau
        if (!it->reussi && plateau_.sontConnectees(idxJoueur, it->villeA, it->villeB)) {
            it->reussi  = true;
            it->visible = true;
            ++ticketsReussisTotal_;

            std::cout << "  *** Ticket! " << joueur.nom << " relie "
                      << it->villeA << " et " << it->villeB << " ! ***\n";

            joueur.ticketsReussis.push_back(*it);
            it = joueur.tickets.erase(it);

            // Piocher un nouveau ticket en remplacement
            if (!piocherUnTicket(idxJoueur))
                std::cout << "  (Plus de tickets à piocher.)\n";
        } else {
            ++it;
        }
    }
}

// ============================================================
//  Fin de partie
// ============================================================

bool Partie::partieTerminee() const {
    for (const auto& j : joueurs_) {
        int total = static_cast<int>(j.ticketsReussis.size()) + (j.aCarteBonus ? 1 : 0);
        if (total >= 6)    return true;
        if (j.nbWagons == 0) return true;
    }
    return false;
}

// ============================================================
//  Affichage
// ============================================================

void Partie::afficherEtat() const {
    std::cout << "\n========================================\n";
    std::cout << "  ÉTAT DE LA PARTIE\n";
    std::cout << "  Pioche cartes  : " << piocheCartes_.size()  << " cartes\n";
    std::cout << "  Pioche tickets : " << piocheTickets_.size() << " tickets\n";
    std::cout << "  Tickets réussis (total) : " << ticketsReussisTotal_ << "\n";
    std::cout << "========================================\n";
    for (const auto& j : joueurs_)
        j.afficher();
    plateau_.afficherVoiesDisponibles();
}

// ============================================================
//  Boucle de jeu
// ============================================================

void Partie::joueurSuivant() {
    joueurCourant_ = (joueurCourant_ + 1) % static_cast<int>(joueurs_.size());
}

bool Partie::jouerTour(int idxJoueur) {
    Joueur& joueur = joueurs_[idxJoueur];

    std::cout << "\n----------------------------------------\n";
    std::cout << "Tour de : " << joueur.nom
              << " (" << joueur.couleurWagon << ")\n";
    joueur.afficher();

    std::cout << "\nActions :\n"
              << "  1 - Piocher 2 cartes Train\n"
              << "  2 - Prendre une voie ferrée\n"
              << "  3 - Passer son tour (changer les tickets)\n"
              << "  0 - Afficher l'état complet\n"
              << "Choix : ";

    int choix = -1;
    std::cin >> choix;

    switch (choix) {
        case 0:
            afficherEtat();
            return false; // tour non consommé

        case 1:
            piocherCartes(idxJoueur);
            return true;

        case 2: {
            plateau_.afficherVoiesDisponibles();
            std::cout << "Index de la voie à prendre : ";
            int idx = -1;
            std::cin >> idx;
            if (prendreVoie(idxJoueur, idx))
                return true;
            std::cout << "  Action impossible, réessayez.\n";
            return false;
        }

        case 3:
            passerTour(idxJoueur);
            return true;

        default:
            std::cout << "  Choix invalide.\n";
            return false;
    }
}

void Partie::jouer() {
    if (joueurs_.empty()) {
        std::cerr << "[Erreur] La partie n'est pas initialisée.\n";
        return;
    }

    std::cout << "\n=== Début de la partie ===\n";
    afficherEtat();

    while (partieEnCours_) {
        bool tourJoue = false;
        while (!tourJoue)
            tourJoue = jouerTour(joueurCourant_);

        if (partieTerminee()) {
            partieEnCours_ = false;
            std::cout << "\n=== FIN DE LA PARTIE ===\n";
            afficherEtat();

            // Annonce du vainqueur
            int maxTickets = -1;
            std::string vainqueur;
            for (const auto& j : joueurs_) {
                int total = static_cast<int>(j.ticketsReussis.size())
                          + (j.aCarteBonus ? 1 : 0);
                if (total > maxTickets) {
                    maxTickets = total;
                    vainqueur  = j.nom;
                }
            }
            std::cout << "Félicitations à " << vainqueur
                      << " qui remporte la partie avec "
                      << maxTickets << " ticket(s) réussi(s) !\n";
        } else {
            joueurSuivant();
        }
    }
}
