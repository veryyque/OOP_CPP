#pragma once

#include "Converter.h"
#include "AudioStream.h"

class MixConverter : public Converter {
    size_t offset_;
    std::vector<int16_t> mix_samples_;
public:
    MixConverter(size_t offset, AudioStream mixStream);
    void apply(const AudioStream& input, AudioStream& output) const override;
};