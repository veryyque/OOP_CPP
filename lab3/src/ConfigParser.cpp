// src/ConfigParser.cpp
#include "../include/ConfigParser.h"
#include <sstream>
#include <fstream>

std::vector<ConfigParser::Command> ConfigParser::parse(const std::string& filename) {
    std::ifstream f(filename);
    if (!f) {
        throw "ConfigParser::parse: Не удалось открыть файл конфигурации: " + filename;
    }

    std::vector<Command> commands;
    std::string line;
    while (std::getline(f, line)) {
        
        size_t start = line.find_first_not_of(" \t");
        if (start == std::string::npos) continue;
        if (line[start] == '#') continue;

        size_t end = line.find_last_not_of(" \t");
        std::string trimmed = line.substr(start, end - start + 1);

        std::stringstream ss(trimmed);
        std::string type;
        ss >> type;
        if (type.empty()) continue;

        std::vector<std::string> params;
        std::string param;
        while (ss >> param) {
            params.push_back(param);
        }

        Command cmd;
        cmd.type = type;
        cmd.params = params;
        commands.push_back(cmd);
    }
    if (commands.empty()) {
        throw "ConfigParser::parse: Файл конфигурации пуст или не содержит правильных команд";
    }
    return commands;
}