#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "carteTrain.hpp"
#include "Models.hpp"

/**
 * @brief Représente un joueur dans la partie.
 *
 * Gère ses wagons, sa main de cartes Train et ses tickets (en cours / réussis).
 */
class Joueur {

private:
    int id;
    std::string nom;
    std::string couleurWagon;   // couleur d'affichage des wagons du joueur
    int nbwagon;
    std::vector<CarteTrain*> mainCartes;
    std::vector<Ticket> ticketsEnMain;
    std::vector<Ticket> ticketsReussis;
    bool aGrandeTraversee;

public:
    Joueur(int id, const std::string& n, const std::string& couleur);

    // --- Getters ---
    int         getId()            const;
    std::string getNom()           const;
    std::string getCouleurWagon()  const;
    int         getNbWagons()      const;
    int         getNbTicketsReussis() const; // inclut Grande Traversée
    bool        hasGrandeTraversee()  const;

    const std::vector<CarteTrain*>& getCartes()        const;
    const std::vector<Ticket>&      getTicketsEnMain() const;
    const std::vector<Ticket>&      getTicketsReussis() const;

    // --- Cartes ---
    void addCarte(CarteTrain* carte);
    void use_wagons(int nb);
    bool peutPrendreVoie(int longueur, Couleur couleur) const;
    std::vector<CarteTrain*> jouerCartes(int longueur, Couleur couleur);

    // --- Tickets ---
    void             ajouterTicket(const Ticket& t);
    bool             completerTicket(int ticketId); // retourne true si trouvé
    std::vector<Ticket> defausserTousTickets();     // vide ticketsEnMain et retourne les tickets

    // --- Grande Traversée ---
    void setGrandeTraversee(bool val);

    // --- Affichage ---
    void afficherEtat() const;
    bool aMoinsDeWagons(int seuil) const;
};