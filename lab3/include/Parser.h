#pragma once

#include <string>
#include <vector>

struct ProgramArgs {
    std::string config_file;
    std::string output_file;
    std::vector<std::string> input_files;
    bool show_help = false;
};

ProgramArgs parse_command_line(int argc, char* argv[]);
void print_usage();