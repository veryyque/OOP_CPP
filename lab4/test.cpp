#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "csv_parser.h"

//тесты конвертации типов
TEST(TypeConversionTest, StringConversion) {
    std::string result = ConvertFromString<std::string>("Hello, World!", 1, 1);
    EXPECT_EQ(result, "Hello, World!");
}

TEST(TypeConversionTest, IntConversion) {
    int result = ConvertFromString<int>("42", 1, 1);
    EXPECT_EQ(result, 42);
    result = ConvertFromString<int>("  123  ", 1, 1);
    EXPECT_EQ(result, 123);
}

TEST(TypeConversionTest, IntConversionError) {
    EXPECT_THROW(ConvertFromString<int>("not_a_number", 1, 1), CsvParseException);
    EXPECT_THROW(ConvertFromString<int>("123abc", 1, 1), CsvParseException);
    EXPECT_THROW(ConvertFromString<int>("", 1, 1), CsvParseException);
}

TEST(TypeConversionTest, DoubleConversion) {
    double result = ConvertFromString<double>("3.14159", 1, 1);
    EXPECT_DOUBLE_EQ(result, 3.14159);
    result = ConvertFromString<double>("1.23e-4", 1, 1);
    EXPECT_DOUBLE_EQ(result, 0.000123);
}

TEST(TypeConversionTest, FloatConversion) {
    float result = ConvertFromString<float>("2.718", 1, 1);
    EXPECT_FLOAT_EQ(result, 2.718f);
}

//тест CSVParser config
TEST(CsvParserConfigTest, DefaultConstructor) {
    CsvParserConfig config;
    EXPECT_EQ(config.ColumnSeparator, ',');
    EXPECT_EQ(config.RowSeparator, '\n');
    EXPECT_EQ(config.quoteChar, '"');
    EXPECT_FALSE(config.hasHeader);
}

TEST(CsvParserConfigTest, ParameterizedConstructor) {
    CsvParserConfig config(';', '\r', '\'', true);
    EXPECT_EQ(config.ColumnSeparator, ';');
    EXPECT_EQ(config.RowSeparator, '\r');
    EXPECT_EQ(config.quoteChar, '\'');
    EXPECT_TRUE(config.hasHeader);
}

TEST(CsvParserConfigTest, NullCharacterHandling) {
    CsvParserConfig config('\0', '\0', '\0', false);
    EXPECT_EQ(config.ColumnSeparator, ',');
    EXPECT_EQ(config.RowSeparator, '\n');
    EXPECT_EQ(config.quoteChar, '"');
}
//тест исключений
TEST(CsvParseExceptionTest, ExceptionMessage) {
    CsvParseException ex("Invalid number", 5, 3);
    EXPECT_EQ(ex.Line(), 5);
    EXPECT_EQ(ex.Column(), 3);

    std::string msg = ex.what();
    EXPECT_NE(msg.find("line 5"), std::string::npos);
    EXPECT_NE(msg.find("column 3"), std::string::npos);
    EXPECT_NE(msg.find("Invalid number"), std::string::npos);
}

//тесты функции печати
TEST(TupleUtilsTest, PrintTupleBasic) {
    std::tuple<> empty_tuple;
    std::ostringstream oss1;
    oss1 << empty_tuple;
    EXPECT_EQ(oss1.str(), "()");
    
    std::tuple<int> single_tuple(42);
    std::ostringstream oss2;
    oss2 << single_tuple;
    EXPECT_EQ(oss2.str(), "(42)");
    
    std::tuple<int, std::string, double> complex_tuple(1, "test", 3.14);
    std::ostringstream oss3;
    oss3 << complex_tuple;
    EXPECT_EQ(oss3.str(), "(1, test, 3.14)");
}

TEST(TupleUtilsTest, PrintTupleWithDifferentTypes) {
    std::tuple<int, bool, char> mixed_tuple(10, true, 'A');
    std::ostringstream oss;
    oss << mixed_tuple;
    EXPECT_EQ(oss.str(), "(10, 1, A)");
}

TEST(ParseFieldTest, UnquotedField) {
    std::stringstream ss;
    CsvParser parser(ss);
    
    std::string line = "field1,field2";
    size_t pos = 0;
    
    std::string field = parser.ParseField(line, pos, 1, 1);
    EXPECT_EQ(field, "field1");
    EXPECT_EQ(pos, 6); //запятая
}


TEST(ParseFieldTest, EscapedQuotes) {
    std::stringstream ss;
    CsvParser parser(ss);
    
    std::string line = "\"Bob \"\"The Boss\"\"\",100";
    size_t pos = 0;
    
    std::string field = parser.ParseField(line, pos, 1, 1);
    EXPECT_EQ(field, "Bob \"The Boss\"");
    EXPECT_EQ(pos, 18);  //после закрывающей кавычки
}

//тесты кофиг
TEST(CsvParserMethodTest, ConfigMethod) {
    std::stringstream ss;
    CsvParserConfig custom_config(';', '\r', '\'', true);
    CsvParser<int, std::string> parser(ss, custom_config);
    
    const CsvParserConfig& config = parser.config_;
    EXPECT_EQ(config.ColumnSeparator, ';');
    EXPECT_EQ(config.RowSeparator, '\r');
    EXPECT_EQ(config.quoteChar, '\'');
    EXPECT_TRUE(config.hasHeader);
}

//тест колонок
TEST(ParseLineTest, DifferentColumnCounts) {
    std::stringstream ss("1,2,3,4,5\n");
    CsvParser<int, int, int> parser(ss);  //3 колонки
    
    auto it = parser.begin();
    EXPECT_EQ(std::get<0>(*it), 1);
    EXPECT_EQ(std::get<1>(*it), 2);
    EXPECT_EQ(std::get<2>(*it), 3);
}
//запятая в конце
TEST(SpecialCasesTest, TrailingComma) {
    std::stringstream ss("1,Alice,100.0,\n2,Bob,200.0,\n");
    CsvParser<int, std::string, double> parser(ss);
    
    auto it = parser.begin();
    ASSERT_NE(it, parser.end());
    EXPECT_EQ(std::get<0>(*it), 1);
    EXPECT_EQ(std::get<1>(*it), "Alice");
    EXPECT_DOUBLE_EQ(std::get<2>(*it), 100.0);
    
    ++it;
    EXPECT_EQ(std::get<0>(*it), 2);
    EXPECT_EQ(std::get<1>(*it), "Bob");
    EXPECT_DOUBLE_EQ(std::get<2>(*it), 200.0);
}
//не обрезает пробелы
TEST(SpecialCasesTest, WhitespaceHandling) {
    std::stringstream ss("  1  ,  Alice  ,  100.5  \n");
    CsvParser<int, std::string, double> parser(ss);
    
    auto it = parser.begin();
    EXPECT_EQ(std::get<0>(*it), 1);
    EXPECT_EQ(std::get<1>(*it), "  Alice  ");
    EXPECT_DOUBLE_EQ(std::get<2>(*it), 100.5);
}

TEST(LargeFileTest, MultipleLines) {
    std::stringstream ss;
    for (int i = 0; i < 1000; ++i) {
        ss << i << ",Name" << i << "," << (i * 1.5) << "\n";
    }
    
    CsvParser<int, std::string, double> parser(ss);
    
    int count = 0;
    for (const auto& row : parser) {
        EXPECT_EQ(std::get<0>(row), count);
        EXPECT_EQ(std::get<1>(row), "Name" + std::to_string(count));
        EXPECT_DOUBLE_EQ(std::get<2>(row), count * 1.5);
        ++count;
    }
    
    EXPECT_EQ(count, 1000);
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}