#ifndef LIFEPARSER_H
#define LIFEPARSER_H

#include <string>

struct LifeFile {
    std::string universeName;
    std::string rule;
    int liveCells[400][2]; //макс 400 клеток
    int liveCellsCount;
};

bool parseRule(const std::string& line, std::string& rule);
void parseString(const std::string& input, int birth[], int& birthCount, int survive[], int& surviveCount);
LifeFile readLifeFile(const std::string& filename, int birth[], int& birthCount, int survive[], int& surviveCount);

#endif