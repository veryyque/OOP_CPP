// src/CommandLineParser.cpp
#include "../include/Parser.h"
#include <iostream>
#include <stdexcept>

void print_usage() {
    std::cout << "Запуск: sound_processor [-h] [-c config.txt output.wav input1.wav [input2.wav ...]]\n\n"
              << "Поддерживаемые конвертеры:\n"
              << "mute: Заглушить интервал времени в секундах.\n"
              << "  Синтаксис: mute <старт> <конец>\n\n"
              << "mix: Смешать с дополнительным потоком, начиная с смещения в секундах.\n"
              << "  Синтаксис: mix $n <старт>\n\n"
              << "robot: Применить эффект голоса робота.\n"
              << "  Синтаксис: robot <частота в Герцах от 1 до 1000>\n\n";
}

ProgramArgs parse_command_line(int argc, char* argv[]) {
    ProgramArgs args;
    if (argc < 2) {
        args.show_help = true;
        return args;
    }

    std::string arg1 = argv[1];
    if (arg1 == "-h" || arg1 == "--help") {
        args.show_help = true;
        return args;
    }

    if (arg1 != "-c") {
        throw "CommandLineParser: Ожидался флаг -c";
    }

    if (argc < 5) {
        throw "CommandLineParser: Использование: sound_processor -c config.txt output.wav input1.wav [input2.wav ...]";
    }

    args.config_file = argv[2];
    args.output_file = argv[3];
    for (int i = 4; i < argc; ++i) {
        args.input_files.push_back(argv[i]);
    }

    if (args.input_files.empty()) {
        throw "CommandLineParser: Не предоставлены входные файлы";
    }

    return args;
}