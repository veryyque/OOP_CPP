#ifndef FIELD_H
#define FIELD_H

#include "Cell.h"
#include <string>

class Field {
public:
    Field();
    void print() const;
    void setCellAlive(int x, int y);
    void update(const int birth[], int birthCount, const int survive[], int surviveCount);
    bool saveToFile(const std::string& filename, const std::string& universeName, const std::string& rule) const;
    bool checkIfIn(const int arr[], int arrSize, int item) const;
    int countLiveNeighbors(int x, int y) const;

private:
    static const int width = 20;
    static const int height = 20;
    Cell cells[height][width];
};

#endif