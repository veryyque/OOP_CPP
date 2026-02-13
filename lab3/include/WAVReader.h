#pragma once

#include "AudioStream.h"
#include <string> 
#include <fstream>        

class WAVReader {
public:
    static AudioStream read(const std::string& filename);
};