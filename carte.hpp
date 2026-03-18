#ifndef CARTE_HPP
#define CARTE_HPP
#include <iostream>
#include <cstring>

using namespace std;

class Carte {

    private:

    string couleur;

    public:

    Carte(string couleur);
    
    string getCouleur();

};




#endif