#include "../include/MixConverter.h"
#include <algorithm>

MixConverter::MixConverter(size_t offset, AudioStream mixStream)
    : offset_(offset), mix_samples_(mixStream.read_all()) {
}

void MixConverter::apply(const AudioStream& input, AudioStream& output) const {
    output.clear();
    
    for (size_t i = 0; i < input.size(); ++i) {
        int16_t main_sample = input.read_sample(i);
        int16_t result_sample = main_sample;
        if (i >= offset_ && (i - offset_) < mix_samples_.size()) {
            int16_t mix_sample = mix_samples_[i - offset_];
            int32_t mixed = (static_cast<int32_t>(main_sample) + static_cast<int32_t>(mix_sample)) / 2;
            mixed = std::clamp(mixed, -32768, 32767);
            result_sample = static_cast<int16_t>(mixed);
        }
        
        output.add_sample(result_sample);
    }
}