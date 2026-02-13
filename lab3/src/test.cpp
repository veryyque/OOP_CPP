#include <gtest/gtest.h>
#include "../include/AudioStream.h"
#include "../include/MuteConverter.h"
#include "../include/MixConverter.h"
#include "../include/RobotVoiceConverter.h"
#include "../include/ConfigParser.h"
#include "../include/ConverterFactory.h"
#include "../include/WAVReader.h"
#include "../include/WAVWriter.h"
#include "../include/Parser.h"
#include <fstream>
#include <cmath>
#include <iostream>

TEST(AudioStreamTest, DefaultConstructor) {
    AudioStream stream;
    EXPECT_TRUE(stream.empty());
    EXPECT_EQ(stream.size(), 0);
    EXPECT_TRUE(stream.is_memory);
    EXPECT_EQ(stream.sample_rate, 44100);
}

TEST(MuteConverterTest, Constructor) {
    MuteConverter converter(100, 200);
    EXPECT_TRUE(true);
}


TEST(MixConverterTest, Constructor) {
    std::vector<int16_t> mix_data = {50, 100};
    AudioStream mix_stream(mix_data);
    
    MixConverter converter(1, std::move(mix_stream));
    EXPECT_TRUE(true);
}

TEST(RobotVoiceConverterTest, Constructor) {
    RobotVoiceConverter converter(5.0, 1, 3);
    EXPECT_TRUE(true);
}

TEST(ConfigParserTest, ParseValidConfig) {
    std::string filename = "test_config.txt";
    std::ofstream file(filename);
    file << "mute 1.0 2.0\n";
    file << "mix $1 0.5\n";
    file << "robot 5.0\n";
    file << "# комментарий\n";
    file << "   mute   3.0   4.0   \n"; //с пробелами
    file.close();
    
    ConfigParser parser;
    auto commands = parser.parse(filename);
    
    EXPECT_EQ(commands.size(), 4);
    EXPECT_EQ(commands[0].type, "mute");
    EXPECT_EQ(commands[0].params[0], "1.0");
    EXPECT_EQ(commands[0].params[1], "2.0");
    
    EXPECT_EQ(commands[1].type, "mix");
    EXPECT_EQ(commands[1].params[0], "$1");
    EXPECT_EQ(commands[1].params[1], "0.5");
    
    std::remove(filename.c_str());
}

TEST(ConverterFactoryTest, CreateMuteConverter) {
    std::vector<std::string> params = {"1.0", "2.0"};
    std::vector<std::string> input_files = {"test1.wav", "test2.wav"};
    
    Converter* converter = ConverterFactory::create("mute", params, input_files);
    
    EXPECT_NE(converter, nullptr);
    EXPECT_TRUE(dynamic_cast<MuteConverter*>(converter) != nullptr);
    
    delete converter;
}

TEST(ConverterFactoryTest, CreateMuteConverterInvalidParams) {
    std::vector<std::string> params = {"1.0"}; //не хватает параметров
    std::vector<std::string> input_files = {"test.wav"};
    
    EXPECT_THROW({
        ConverterFactory::create("mute", params, input_files);
    }, const char*);
}

int main(int argc, char **argv) {
    std::cout << "Starting tests..." << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    std::cout << "Tests finished." << std::endl;
    return result;
}