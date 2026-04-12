#pragma once

#include <vector>
#include <deque>
#include <string>
#include "board.hpp"
#include "Joueur.hpp"

// classe principale qui gere le jeu
class Jeu {
private:
    Board board;
    std::vector<Joueur*>     joueurs;
    std::vector<CarteTrain*> toutesLesCartes; // ownership de toutes les cartes
    std::deque<CarteTrain*>  piocheCartes;
    std::deque<Ticket>       piocheTickets;
    std::vector<CarteTrain*> defausseCartes;
    std::vector<Ticket>      defausseTickets;

    int  joueurCourantIdx;
    bool partieTerminee;
    bool grandeTraverseeAttribuee;

    // villes côte ouest / est pour la grande traversée
    static const std::vector<std::string> villesOuest;
    static const std::vector<std::string> villesEst;

public:
    Jeu();
    ~Jeu();

    void initialiser(int nbJoueurs);
    void lancerPartie();

private:
    void creerPiocheCartes();
    void melangerCartes();
    void distribuerDepart();

    void jouerTour(Joueur& j);
    void actionPiocherCartes(Joueur& j);
    void actionPrendreVoie(Joueur& j);
    void actionEchangerTickets(Joueur& j);

    CarteTrain* piocherCarte();
    Ticket      piocherTicket();

    void verifierTickets(Joueur& j);
    bool checkChemin(const std::string& a, const std::string& b, int pid);
    void verifierGrandeTraversee(Joueur& j);

    void afficherEtat();
    void afficherResultats();
};
