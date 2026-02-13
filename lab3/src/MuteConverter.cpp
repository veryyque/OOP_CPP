#include "../include/MuteConverter.h"
#include <algorithm>

MuteConverter::MuteConverter(size_t start, size_t end) 
    : start_(start), end_(end) {
}

void MuteConverter::apply(const AudioStream& input, AudioStream& output) const {
    output.clear();
    
    for (size_t i = 0; i < input.size(); ++i) {
        int16_t sample = input.read_sample(i);
    
        if (i >= start_ && i < end_) {
            sample = 0;
        }
        
        output.add_sample(sample);
    }
}
