#pragma once

#include <string>
#include <vector>

class ConfigParser {
public:
    struct Command {
        std::string type;
        std::vector<std::string> params;
    };
    std::vector<Command> parse(const std::string& filename);
};