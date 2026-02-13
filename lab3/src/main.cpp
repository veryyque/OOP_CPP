#include "../include/Parser.h"
#include "../include/WAVReader.h"
#include "../include/WAVWriter.h"
#include "../include/ConfigParser.h"
#include "../include/ConverterFactory.h"
#include <iostream>
#include <memory>
#include <string>

int main(int argc, char* argv[]) {
    try {
        ProgramArgs args = parse_command_line(argc, argv);
        if (args.show_help) {
            print_usage();
            return 0;
        }

        std::cout << "=== Запуск Sound Processor ===" << std::endl;
        
        ConfigParser parser;
        auto commands = parser.parse(args.config_file);
        std::cout << "Команд в конфиге: " << commands.size() << std::endl;

        AudioStream current = WAVReader::read(args.input_files[0]);
        std::cout << "Исходный файл: " << current.size() << " сэмплов" << std::endl;

        for (size_t i = 0; i < commands.size(); ++i) {
            const auto& cmd = commands[i];
            std::cout << "[" << (i+1) << "/" << commands.size() << "] Применение: " << cmd.type;
    
            for (const auto& param : cmd.params) {
                std::cout << " " << param;
            }
            std::cout << std::endl;
            
            auto converter = ConverterFactory::create(cmd.type, cmd.params, args.input_files);
    
            AudioStream result;
            
            converter->apply(current, result);
            
            current = std::move(result);
            
            delete converter;
            std::cout << "  Результат: " << current.size() << " сэмплов" << std::endl;
        }

        std::cout << "Запись в файл: " << args.output_file << std::endl;
        WAVWriter::write(args.output_file, current);
        std::cout << "=== Обработка завершена ===" << std::endl;
        
    } catch (const char* e) {
        std::cerr << "ОШИБКА: " << e << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "ОШИБКА: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "НЕИЗВЕСТНАЯ ОШИБКА" << std::endl;
        return 1;
    }

    return 0;
}