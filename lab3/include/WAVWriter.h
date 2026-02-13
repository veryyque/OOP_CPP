#pragma once

#include "AudioStream.h"
#include <string>
#include <fstream>
#include <vector>

class WAVWriter {
public:
    static void write(const std::string& filename, const AudioStream& stream);
    static void write(const std::string& filename, const std::vector<int16_t>& samples);
};