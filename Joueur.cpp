#include "Joueur.hpp"

<<<<<<< HEAD



//Constructeur

Joueur :: Joueur(std :: string n)   {
    nom = n;
    nbwagon = 20;
}
Joueur :: Joueur(std :: string n, std :: vector<CarteTrain*> m_carte,  std::vector<ticket*> m_ticket)   {
    nom = n;
    Main_carte = m_carte;
    Main_ticket = m_ticket;


=======
// Constructeur
Joueur::Joueur(std::string n) : nom(n), nbwagon(20), score(0), nbticketsreussis(0) {
>>>>>>> 87a31db466ccb5142f2c5f7f849db0d537df9a7e
}

// Getters
std::string Joueur::getNom() const {
    return nom;
}

<<<<<<< HEAD
// Getters

std::string Joueur :: getNom() const  {
    return nom;

}
int Joueur :: get_nbwagons()const {

    return nbwagon;
}

int Joueur :: get_Nbticketsreussis()const  {

    return nbticketsreussis;
}


//Methodes


void Joueur :: add_newticket()  {
    
    this->Main_ticket.push_back(new ticket);

}


void Joueur :: add_newcarte()   {
    
    this->Main_carte.push_back(new CarteTrain());
}


    void Joueur :: use_wagons(int nb) {
        if (nbwagon) {
            nbwagon--;
        }
=======
int Joueur::getNbWagons() const {
    return nbwagon;
}

int Joueur::getScore() const {
    return score;
}

int Joueur::getNbTicketsReussis() const {
    return nbticketsreussis;
}

const std::vector<CarteTrain*>& Joueur::getCartes() const {
    return Main_carte;
}

// Methodes
void Joueur::addCarte(CarteTrain* carte) {
    Main_carte.push_back(carte);
}

void Joueur::addWagons(int nb) {
    nbwagon += nb;
}

void Joueur::use_wagons(int nb) {
    if (nbwagon >= nb) {
        nbwagon -= nb;
    }
}

bool Joueur::peutPrendreVoie(int longueur, Couleur couleur) const {
    if (nbwagon < longueur) return false;

    int nbCouleur = 0;
    int nbLoco = 0;

    for (const CarteTrain* c : Main_carte) {
        if (c->getCarteTrain() == Couleur::Multicolore) {
            nbLoco++;
        } else if (c->getCarteTrain() == couleur) {
            nbCouleur++;
        }
    }

    return (nbCouleur + nbLoco) >= longueur;
}

std::vector<CarteTrain*> Joueur::jouerCartes(int longueur, Couleur couleur) {
    std::vector<CarteTrain*> defausse;

    if (!peutPrendreVoie(longueur, couleur)) {
        std::cout << "[ERREUR] " << nom << " ne peut pas prendre cette voie.\n";
        return defausse;
>>>>>>> 87a31db466ccb5142f2c5f7f849db0d537df9a7e
    }

    int aPoser = longueur;

    // Passe 1 : on joue les cartes de la bonne couleur en priorite
    for (auto it = Main_carte.begin(); it != Main_carte.end() && aPoser > 0; ) {
        if ((*it)->getCarteTrain() == couleur) {
            defausse.push_back(*it);
            it = Main_carte.erase(it);
            aPoser--;
        } else {
            ++it;
        }
    }

    // Passe 2 : on complete avec des locomotives si necessaire
    for (auto it = Main_carte.begin(); it != Main_carte.end() && aPoser > 0; ) {
        if ((*it)->getCarteTrain() == Couleur::Multicolore) {
            defausse.push_back(*it);
            it = Main_carte.erase(it);
            aPoser--;
        } else {
            ++it;
        }
    }

    use_wagons(longueur);
    return defausse;
}

void Joueur::incrementerScore(int points) {
    score += points;
}

void Joueur::incrementerTicketsReussis() {
    nbticketsreussis++;
}

bool Joueur::aMoinsDeWagons(int seuil) const {
    return nbwagon <= seuil;
}