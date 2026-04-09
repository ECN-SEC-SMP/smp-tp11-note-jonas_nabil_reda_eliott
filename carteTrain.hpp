#pragma once

#include "Models.hpp"

class CarteTrain {
private:
    Couleur couleur;

public:
    CarteTrain(Couleur c);
    Couleur getCarteTrain() const;
};