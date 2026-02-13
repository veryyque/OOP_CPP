#pragma once

#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>

struct AudioStream {
    std::unique_ptr<std::ifstream> file;
    std::vector<int16_t> samples;
    size_t data_offset;  //начало чанка дата
    size_t num_samples;
    bool is_memory;  //true для in-memory,false для streaming
    static constexpr int sample_rate = 44100;
    
    AudioStream() : data_offset(0), num_samples(0), is_memory(true) {}
    
    //для тестов
    AudioStream(std::vector<int16_t> s) : samples(std::move(s)), data_offset(0), num_samples(s.size()), is_memory(true) {}
    
    AudioStream(std::unique_ptr<std::ifstream> f, size_t offset, size_t samples) 
        : file(std::move(f)), samples(), data_offset(offset), num_samples(samples), is_memory(false) {}
    
    int16_t read_sample(size_t index) const {
        if (index >= num_samples) return 0;
        
        if (is_memory) {
            return samples[index];
        }
        
        if (!file) return 0;
        
        std::streampos pos = data_offset + index * 2; // 2 байта на сэмпл
        file->seekg(pos); //переместить указатель
        
        char buf[2];
        file->read(buf, 2);
        if (file->gcount() != 2) return 0; //gcount количество прочитанных данных
        
        return static_cast<int16_t>(
            static_cast<uint16_t>(static_cast<unsigned char>(buf[0])) | 
            (static_cast<uint16_t>(static_cast<unsigned char>(buf[1])) << 8)
        );
    }
    
    std::vector<int16_t> read_range(size_t start, size_t end) const {
        std::vector<int16_t> result;
        size_t actual_end = std::min(end, num_samples);
        
        if (start >= actual_end) {
            return result;
        }
        
        if (is_memory) {
            result.assign(samples.begin() + start, samples.begin() + actual_end);
        } else {
            result.reserve(actual_end - start);
            for (size_t i = start; i < actual_end; ++i) {
                result.push_back(read_sample(i));
            }
        }
        return result;
    }
    
    std::vector<int16_t> read_all() const {
        return read_range(0, num_samples);
    }
    
    void add_sample(int16_t sample) {
        samples.push_back(sample);
        num_samples = samples.size();
        is_memory = true;
    }
    
    void add_samples(const std::vector<int16_t>& new_samples) {
        samples.insert(samples.end(), new_samples.begin(), new_samples.end());
        num_samples = samples.size();
        is_memory = true;
    }

    void clear() {
        samples.clear();
        num_samples = 0;
        file.reset();
        data_offset = 0;
        is_memory = true;
    }

    size_t size() const { return num_samples; }
    bool empty() const { return num_samples == 0; }
};