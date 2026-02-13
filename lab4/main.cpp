#include "csv_parser.h"
#include <iostream>
#include <fstream>

int main() {
    try {
        std::ifstream input1("test1.csv");
        if (!input1.is_open()) {
            std::cerr << "Ошибка: не удалось открыть файл test1.csv" << std::endl;
            return 1;
        }

        CsvParser<int, std::string, double> parser1(input1, 0);

        std::cout << "Содержимое test1.csv (обычный CSV с запятыми и \"):\n";
        for (const auto& row : parser1) {
            std::cout << row << std::endl;
        }
        std::cout << std::endl;

        std::ifstream input2("test2.csv");
        if (!input2.is_open()) {
            std::cerr << "Ошибка: не удалось открыть файл test2.csv" << std::endl;
            return 1;
        }

        CsvParserConfig customConfig;
        customConfig.ColumnSeparator = ';';
        customConfig.quoteChar = '^';
        customConfig.RowSeparator = '\n';
        customConfig.hasHeader = true;

        CsvParser<int, std::string, double> parser2(input2, customConfig);

        std::cout << "Содержимое test2.csv (разделитель ;, кавычки ^, пропущен заголовок):\n";
        for (const auto& row : parser2) {
            std::cout << row << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}