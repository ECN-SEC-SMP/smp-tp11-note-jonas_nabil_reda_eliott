#include "carteTrain.hpp"

CarteTrain::CarteTrain(Couleur couleur){
    this->couleur = couleur;
}

Couleur CarteTrain::getCarteTrain() const{
    return (this->couleur);
}