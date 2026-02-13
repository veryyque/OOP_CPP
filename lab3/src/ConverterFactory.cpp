// src/ConverterFactory.cpp
#include "../include/ConverterFactory.h"
#include "../include/MuteConverter.h"
#include "../include/MixConverter.h"
#include "../include/RobotVoiceConverter.h"
#include "../include/WAVReader.h"

Converter* ConverterFactory::create(const std::string& type, const std::vector<std::string>& params, const std::vector<std::string>& input_files) {
    if (type == "mute") {
        if (params.size() != 2) {
            throw "ConverterFactory::create (mute): Требуется ровно 2 параметра";
        }
        double start_s, end_s;
        try {
            start_s = std::stod(params[0]);
            end_s = std::stod(params[1]);
        } catch (...) {
            throw "ConverterFactory::create (mute): Неверные параметры (не числа)";
        }
        if (start_s < 0 || end_s <= start_s) {
            throw "ConverterFactory::create (mute): Неверный интервал (начало >=0, конец > начало)";
        }
        size_t start = static_cast<size_t>(start_s * AudioStream::sample_rate + 0.5);
        size_t end = static_cast<size_t>(end_s * AudioStream::sample_rate + 0.5);
        return new MuteConverter(start, end);
    } else if (type == "mix") {
        if (params.size() != 2) {
            throw "ConverterFactory::create (mix): Требуется ровно 2 параметра";
        }
        const std::string& aux_ref = params[0];
        if (aux_ref.size() < 2 || aux_ref[0] != '$') {
            throw "ConverterFactory::create (mix): Первый параметр должен быть $n (ссылка на файл)";
        }
        int n;
        try {
            n = std::stoi(aux_ref.substr(1));
        } catch (...) {
            throw "ConverterFactory::create (mix): Неверный формат $n";
        }
        if (n < 1 || static_cast<size_t>(n) > input_files.size()) {
            throw "ConverterFactory::create (mix): Неверный индекс файла (n от 1 до количества входов)";
        }
        try {
            AudioStream aux = WAVReader::read(input_files[n - 1]);
            double offset_s = std::stod(params[1]);
            if (offset_s < 0) {
                throw "ConverterFactory::create (mix): Смещение должно быть >=0";
            }
            size_t offset = static_cast<size_t>(offset_s * AudioStream::sample_rate + 0.5);
            return new MixConverter(offset, std::move(aux));
        } catch (...) {
            throw "ConverterFactory::create (mix): Ошибка чтения вспомогательного файла или неверное смещение";
        }
    } else if (type == "robot") {
        if (params.size() != 1 && params.size() != 3) {
            throw "ConverterFactory::create (robot): Требуется 1 или 3 параметра (freq [start_sec end_sec])";
        }
        double freq;
        size_t start = 0;
        size_t end = SIZE_MAX;
        try {
            freq = std::stod(params[0]);
        } catch (...) {
            throw "ConverterFactory::create (robot): Неверная частота модуляции (не число)";
        }
        if (freq <= 0 || freq > 1000) {
            throw "ConverterFactory::create (robot): Частота модуляции должна быть от 1 до 1000 Гц";
        }
        if (params.size() == 3) {
            double start_s, end_s;
            try {
                start_s = std::stod(params[1]);
                end_s = std::stod(params[2]);
            } catch (...) {
                throw "ConverterFactory::create (robot): Неверные параметры интервала (не числа)";
            }
            if (start_s < 0 || end_s <= start_s) {
                throw "ConverterFactory::create (robot): Неверный интервал (начало >=0, конец > начало)";
            }
            start = static_cast<size_t>(start_s * AudioStream::sample_rate + 0.5);
            end = static_cast<size_t>(end_s * AudioStream::sample_rate + 0.5);
        }
        return new RobotVoiceConverter(freq);
    } else {
        throw "ConverterFactory::create: Неизвестный конвертер: " + type;
    }
}