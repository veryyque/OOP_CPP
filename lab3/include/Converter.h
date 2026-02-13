#pragma once
#include "AudioStream.h"

class Converter {
public:
    virtual ~Converter() = default;
    virtual void apply(const AudioStream& input, AudioStream& output) const = 0;
    AudioStream apply(const AudioStream& input) const {
        AudioStream output;
        apply(input, output);
        return output;
    }
};