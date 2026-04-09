#include "carteTrain.hpp"

CarteTrain::CarteTrain(Couleur c) : couleur(c) {}

Couleur CarteTrain::getCarteTrain() const {
    return couleur;
}