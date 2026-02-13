#include "Cell.h"

Cell::Cell() : isAlive(false) {
}

void Cell::makeAlive() {
    isAlive = true;
}

void Cell::makeDead() {
    isAlive = false;
}

bool Cell::isCellAlive() const {
    return isAlive;
}
