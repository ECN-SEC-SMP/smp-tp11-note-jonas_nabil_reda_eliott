#pragma once

#include "Plateau.hpp"
#include <vector>
#include <string>
#include <random>

// ============================================================
//  Partie – représente une partie en cours
// ============================================================

class Partie {
public:
    // ---- Construction ----
    Partie();

    /// Initialise la partie : charge les fichiers CSV et crée les joueurs (2–4)
    /// Retourne false si l'initialisation échoue
    bool initialiser(const std::vector<std::string>& nomsJoueurs,
                     const std::string& cheminVoiesCSV,
                     const std::string& cheminTicketsCSV);

    // ---- Boucle de jeu principale ----

    /// Lance la boucle de jeu interactive (console)
    void jouer();

    // ---- Actions d'un tour ----

    /// Action 1 : pioche deux cartes Train
    void piocherCartes(int idxJoueur);

    /// Action 2 : prendre une voie ferrée (identifiée par son index dans le plateau)
    /// Retourne false si l'action est impossible
    bool prendreVoie(int idxJoueur, int idxVoie);

    /// Action 3 : passer son tour (défausser les deux tickets, en reprendre deux)
    void passerTour(int idxJoueur);

    // ---- Vérifications ----

    /// Vérifie et met à jour tous les tickets du joueur
    void mettreAJourTickets(int idxJoueur);

    /// Retourne true si la condition de fin de partie est atteinte
    bool partieTerminee() const;

    // ---- Affichage ----
    void afficherEtat() const;

private:
    // ---- Plateau de jeu ----
    Plateau plateau_;

    // ---- Pioches / défausses ----
    std::vector<CarteTrain> piocheCartes_;
    std::vector<CarteTrain> defausseCartes_;
    std::vector<Ticket>     piocheTickets_;
    std::vector<Ticket>     defausseTickets_;

    // ---- Joueurs ----
    std::vector<Joueur> joueurs_;
    int joueurCourant_ = 0;

    // ---- État global ----
    int  ticketsReussisTotal_ = 0;
    bool partieEnCours_       = true;

    // ---- Générateur aléatoire ----
    std::mt19937 rng_;

    // ---- Helpers privés ----
    void melangerPiocheCartes();
    void melangerPiocheTickets();
    void rechargerPiocheCartes();
    void rechargerPiocheTickets();

    bool piocherUneCarte(int idxJoueur);
    bool piocherUnTicket(int idxJoueur);

    void joueurSuivant();

    /// Joue un tour pour le joueur courant ; retourne true si le tour est consommé
    bool jouerTour(int idxJoueur);
};
