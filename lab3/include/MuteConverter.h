#pragma once

#include "Converter.h"
#include "AudioStream.h"

class MuteConverter : public Converter {
    size_t start_;
    size_t end_;
public:
    MuteConverter(size_t start, size_t end);
    void apply(const AudioStream& input, AudioStream& output) const override;
};