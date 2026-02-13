// include/ConverterFactory.h
#pragma once

#include "Converter.h"
#include <string>
#include <vector>

class ConverterFactory {
public:
    static Converter* create(const std::string& type, const std::vector<std::string>& params, const std::vector<std::string>& input_files);
};