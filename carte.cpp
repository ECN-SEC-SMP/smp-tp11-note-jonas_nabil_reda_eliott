#include "carte.hpp"

Carte::Carte(string couleur){
    this->couleur = couleur;
}

string Carte::getCouleur(){
    return this->couleur;
}