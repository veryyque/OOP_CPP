#pragma once

#include "Converter.h"
#include "AudioStream.h"

class RobotVoiceConverter : public Converter {
    double frequency_;
    size_t start_;
    size_t end_;
    
public:
    RobotVoiceConverter(double freq, size_t start_time = 0, size_t end_time = 0);
    void apply(const AudioStream& input, AudioStream& output) const override;
};