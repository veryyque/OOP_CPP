#include "../include/WAVReader.h"
#include <cstring> 
#include <cstdint> 
#include <iostream>

AudioStream WAVReader::read(const std::string& filename) {
    auto file = std::make_unique<std::ifstream>(filename, std::ios::binary);
    if (!file->is_open()) {
        throw "WAVReader::read: Не удалось открыть файл: " + filename;
    }

    //RIFF
    char header[4];
    file->read(header, 4);
    if (std::strncmp(header, "RIFF", 4) != 0) {
        throw "WAVReader::read: Файл не является WAV";
    }

    //размер файла
    file->ignore(4);

    //WAVE
    file->read(header, 4);
    if (std::strncmp(header, "WAVE", 4) != 0) {
        throw "WAVReader::read: Файл не является WAVE";
    }

    //fmt data чанки
    bool fmt_found = false;
    size_t data_offset = 0;
    size_t data_size = 0;
    
    while (file->good()) {
        file->read(header, 4);
        if (!file->good()) break;

        char size_buf[4];
        file->read(size_buf, 4);
        uint32_t chunk_size = 
            static_cast<uint32_t>(static_cast<unsigned char>(size_buf[0])) |
            static_cast<uint32_t>(static_cast<unsigned char>(size_buf[1])) << 8 |
            static_cast<uint32_t>(static_cast<unsigned char>(size_buf[2])) << 16 |
            static_cast<uint32_t>(static_cast<unsigned char>(size_buf[3])) << 24;

        if (std::strncmp(header, "fmt ", 4) == 0) {
            // Читаем 16 байт fmt
            char fmt_data[16];
            file->read(fmt_data, 16);
            
            uint16_t format = 
                static_cast<uint16_t>(static_cast<unsigned char>(fmt_data[0])) |
                static_cast<uint16_t>(static_cast<unsigned char>(fmt_data[1])) << 8;
            
            uint16_t channels = 
                static_cast<uint16_t>(static_cast<unsigned char>(fmt_data[2])) |
                static_cast<uint16_t>(static_cast<unsigned char>(fmt_data[3])) << 8;
            
            uint32_t sample_rate = 
                static_cast<uint32_t>(static_cast<unsigned char>(fmt_data[4])) |
                static_cast<uint32_t>(static_cast<unsigned char>(fmt_data[5])) << 8 |
                static_cast<uint32_t>(static_cast<unsigned char>(fmt_data[6])) << 16 |
                static_cast<uint32_t>(static_cast<unsigned char>(fmt_data[7])) << 24;
            
            uint16_t bits_per_sample = 
                static_cast<uint16_t>(static_cast<unsigned char>(fmt_data[14])) |
                static_cast<uint16_t>(static_cast<unsigned char>(fmt_data[15])) << 8;

            if (format != 1) throw "WAVReader::read: Не PCM формат";
            if (channels != 1) throw "WAVReader::read: Не моно";
            if (sample_rate != 44100) throw "WAVReader::read: Не 44100 Hz";
            if (bits_per_sample != 16) throw "WAVReader::read: Не 16-bit";

            
            if (chunk_size > 16) {
                file->ignore(static_cast<std::streamsize>(chunk_size - 16));
            }
            fmt_found = true;
        } 
        else if (std::strncmp(header, "data", 4) == 0) {
            if (!fmt_found) throw "WAVReader::read: fmt чанк не найден перед data";
            
            data_offset = file->tellg();  //текущая позиция начала данных
            data_size = chunk_size;
            break;
        } 
        else {
            //пропуск других чанков
            file->ignore(static_cast<std::streamsize>(chunk_size));
        }
    }

    if (data_size == 0) {
        throw "WAVReader::read: data чанк не найден";
    }

    size_t num_samples = data_size / 2;
    std::cout << "Загружен файл: " << filename << ", сэмплов: " << num_samples << std::endl;
    
    return AudioStream(std::move(file), data_offset, num_samples);
}