#include "../include/RobotVoiceConverter.h"
#include <cmath>
#include <algorithm>

RobotVoiceConverter::RobotVoiceConverter(double freq, size_t start_seconds, size_t end_seconds){
    frequency_ = freq;
    start_ = start_seconds;
    end_ = end_seconds;
}

void RobotVoiceConverter::apply(const AudioStream& input, AudioStream& output) const {
    output.clear();
    
    size_t start_sample = start_ * AudioStream::sample_rate;
    size_t end_sample = (end_ == 0) ? input.size() : 
                       std::min(end_ * AudioStream::sample_rate, input.size());
    
    double phase_step = 2.0 * M_PI * frequency_ / AudioStream::sample_rate;

    for (size_t i = 0; i < input.size(); ++i) {
        int16_t sample = input.read_sample(i);
    
        if (i >= start_sample && i < end_sample) {
            double phase = i * phase_step;
            double modulation = std::sin(phase);
            int32_t modulated = static_cast<int32_t>(sample * modulation);
            modulated = std::clamp(modulated, -32768, 32767);
            sample = static_cast<int16_t>(modulated);
        }
        
        output.add_sample(sample);
    }
}
