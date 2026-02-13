#include "../include/WAVWriter.h"
#include <cstdint>
#include <iostream>

void WAVWriter::write(const std::string& filename, const AudioStream& stream) {
    std::ofstream f(filename, std::ios::binary);
    if (!f) {
        throw "WAVWriter::write: Не удалось записать в файл: " + filename;
    }

    //читаем все данные из потока
    auto samples = stream.read_all();
    std::cout << "Запись " << samples.size() << " сэмплов в " << filename << std::endl;

    uint32_t data_size = static_cast<uint32_t>(samples.size()) * 2;
    uint32_t riff_size = 36 + data_size;

    //RIFF заголовок
    f.write("RIFF", 4);
    
    //Размер файла
    char size_buf[4];
    size_buf[0] = static_cast<char>(riff_size & 0xFF);
    size_buf[1] = static_cast<char>((riff_size >> 8) & 0xFF);
    size_buf[2] = static_cast<char>((riff_size >> 16) & 0xFF);
    size_buf[3] = static_cast<char>((riff_size >> 24) & 0xFF);
    f.write(size_buf, 4);

    f.write("WAVE", 4);

    //fmt чанк
    f.write("fmt ", 4);
    
    //размер fmt чанка
    uint32_t fmt_size = 16;
    size_buf[0] = static_cast<char>(fmt_size & 0xFF);
    size_buf[1] = static_cast<char>((fmt_size >> 8) & 0xFF);
    size_buf[2] = static_cast<char>((fmt_size >> 16) & 0xFF);
    size_buf[3] = static_cast<char>((fmt_size >> 24) & 0xFF);
    f.write(size_buf, 4);

    //AudioFormat = 1 (PCM)
    char audio_format[2] = {1, 0};
    f.write(audio_format, 2);

    //NumChannels = 1
    f.write(audio_format, 2);

    //SampleRate = 44100
    uint32_t sample_rate = 44100;
    size_buf[0] = static_cast<char>(sample_rate & 0xFF);
    size_buf[1] = static_cast<char>((sample_rate >> 8) & 0xFF);
    size_buf[2] = static_cast<char>((sample_rate >> 16) & 0xFF);
    size_buf[3] = static_cast<char>((sample_rate >> 24) & 0xFF);
    f.write(size_buf, 4);

    //ByteRate = 44100 * 1 * 16/8 = 88200
    uint32_t byte_rate = 88200;
    size_buf[0] = static_cast<char>(byte_rate & 0xFF);
    size_buf[1] = static_cast<char>((byte_rate >> 8) & 0xFF);
    size_buf[2] = static_cast<char>((byte_rate >> 16) & 0xFF);
    size_buf[3] = static_cast<char>((byte_rate >> 24) & 0xFF);
    f.write(size_buf, 4);

    //BlockAlign = 2
    char block_align[2] = {2, 0};
    f.write(block_align, 2);

    //BitsPerSample = 16
    char bits_per_sample[2] = {16, 0};
    f.write(bits_per_sample, 2);

    //data чанк
    f.write("data", 4);
    
    //размер данных
    size_buf[0] = static_cast<char>(data_size & 0xFF);
    size_buf[1] = static_cast<char>((data_size >> 8) & 0xFF);
    size_buf[2] = static_cast<char>((data_size >> 16) & 0xFF);
    size_buf[3] = static_cast<char>((data_size >> 24) & 0xFF);
    f.write(size_buf, 4);

    //данные
    for (int16_t sample : samples) {
        char sample_buf[2];
        sample_buf[0] = static_cast<char>(sample & 0xFF);
        sample_buf[1] = static_cast<char>((sample >> 8) & 0xFF);
        f.write(sample_buf, 2);
    }

    std::cout << "Файл успешно записан: " << filename << std::endl;
}