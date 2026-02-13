#ifndef CELL_H
#define CELL_H

class Cell {
public:
    Cell();
    void makeAlive();
    void makeDead();
    bool isCellAlive() const;
private:
    bool isAlive;
};

#endif