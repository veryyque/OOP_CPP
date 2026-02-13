#include "Field.h"
#include <iostream>
#include <fstream>

using namespace std;

Field::Field() {
    //все клетки мертвые
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            cells[y][x] = Cell();
        }
    }
}

void Field::print() const {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (cells[i][j].isCellAlive()) {
                cout << '*';
            } else {
                cout << ' ';
            }
            cout << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

void Field::setCellAlive(int x, int y) {
    x = (x % width + width) % width;
    y = (y % height + height) % height;
    cells[y][x].makeAlive();
}

void Field::update(const int birth[], int birthCount, const int survive[], int surviveCount) {
    bool newStates[height][width]; //создаем временный массив для новых состояний

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            newStates[y][x] = false;
        }
    }
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int liveNeighbors = countLiveNeighbors(x, y);
            bool currentAlive = cells[y][x].isCellAlive();
            
            if (currentAlive) { //выживает если кол-во соседей в массиве survive
                newStates[y][x] = checkIfIn(survive, surviveCount, liveNeighbors);
            } else {//рождается если кол-во соседей в birth
                newStates[y][x] = checkIfIn(birth, birthCount, liveNeighbors);
            }
        }
    }
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (newStates[y][x]) {
                cells[y][x].makeAlive();
            } else {
                cells[y][x].makeDead();
            }
        }
    }
}

bool Field::saveToFile(const string& filename, const string& universeName, const string& rule) const {
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Ошибка открытия файла для записи: " + filename);
    }
    
    file << "#Life 1.06" << endl;
    file << "#N " << universeName << endl;
    file << "#R " << rule << endl;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (cells[y][x].isCellAlive()) {
                file << x << " " << y << endl;
            }
        }
    }
    
    file.close();
    return true;
}

int Field::countLiveNeighbors(int x, int y) const {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue; //пропустить саму клетку
            
            int nx = (x + dx + width) % width;
            int ny = (y + dy + height) % height;
            
            if (cells[ny][nx].isCellAlive()) {
                count++;
            }
        }
    }
    return count;
}

bool Field::checkIfIn(const int arr[], int arrSize, int item) const {
    for (int i = 0; i < arrSize; i++) {
        if (arr[i] == item) {
            return true;
        }
    }
    return false;
}