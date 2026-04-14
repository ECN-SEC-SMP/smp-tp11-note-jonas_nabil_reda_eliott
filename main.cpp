// main.cpp
#include <iostream>
#include <limits>
#include "Jeu.hpp"
using namespace std;

int main() {
    cout << "==============================================\n";
    cout << "  Les Aventuriers du Rail - Mon Premier Voyage\n";
    cout << "==============================================\n\n";

    int nbJoueurs = 0;
    while (nbJoueurs < 2 || nbJoueurs > 4) {
        cout << "Nombre de joueurs (2 a 4) : ";
        if (!(cin >> nbJoueurs)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    Jeu jeu;
    jeu.initialiser(nbJoueurs);
    jeu.lancerPartie();

    return 0;
}