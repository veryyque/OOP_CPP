#include "LifeParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

using namespace std;

bool parseRule(const string& line, string& rule) {
    if (line.size() < 3 || line.substr(0, 3) != "#R ") {
        return false;
    }
    rule = line.substr(3);
    return true;
}

void parseString(const string& input, int birth[], int& birthCount, int survive[], int& surviveCount) {
    birthCount = 0;
    surviveCount = 0;

    int bIndex = -1;
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == 'B') {
            bIndex = i;
            break;
        }
    }
 
    int sIndex = -1;
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == 'S') {
            sIndex = i;
            break;
        }
    }

    if (bIndex != -1) {
        for (int i = bIndex + 1; i < input.length(); i++) {
            if (input[i] == 'S') break;
            if (isdigit(input[i])) {
                birth[birthCount++] = input[i] - '0';
            }
        }
    }

    if (sIndex != -1) {
        for (int i = sIndex + 1; i < input.length(); i++) {
            if (isdigit(input[i])) {
                survive[surviveCount++] = input[i] - '0';
            }
        }
    }

    if (birthCount == 0 && surviveCount == 0) {
        birth[birthCount++] = 3;
        survive[surviveCount++] = 2;
        survive[surviveCount++] = 3;
    }
}

LifeFile readLifeFile(const string& filename, int birth[], int& birthCount, int survive[], int& surviveCount) {
    ifstream file(filename);
    LifeFile lifeFile;
    lifeFile.liveCellsCount = 0;
    
    if (!file.is_open()) {
        lifeFile.universeName = "Glider";
        lifeFile.rule = "B3/S23";
        parseString("B3/S23", birth, birthCount, survive, surviveCount);
        return lifeFile;
    }

    string line;
    bool hasName = false;
    bool hasRule = false;

    while (getline(file, line)) {
        if (line.empty()) continue;

        size_t start = line.find_first_not_of(" \t"); //первый не пробел
        if (start == string::npos) continue; //no position константа
        line = line.substr(start);

        if (line.substr(0, 9) == "#Life 1.06") {
            continue;
        } else if (line.substr(0, 3) == "#N ") {
            lifeFile.universeName = line.substr(3);
            hasName = true;
        } else if (line.substr(0, 3) == "#R ") {
            lifeFile.rule = line.substr(3);
            hasRule = true;
            parseString(lifeFile.rule, birth, birthCount, survive, surviveCount);
        } else if (line[0] != '#') {  //координаты
            istringstream iss(line); //строку в поток
            int x, y;
            if (iss >> x >> y && lifeFile.liveCellsCount < 400) {
                lifeFile.liveCells[lifeFile.liveCellsCount][0] = x;
                lifeFile.liveCells[lifeFile.liveCellsCount][1] = y;
                lifeFile.liveCellsCount++;
            }
        }
    }
    
    file.close();
    
    if (!hasName) lifeFile.universeName = "Unknown";
    if (!hasRule) {
        lifeFile.rule = "B3/S23";
        parseString("B3/S23", birth, birthCount, survive, surviveCount);
    }
    
    return lifeFile;
}