#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <string>
#include <vector>
#include <type_traits>
#include <stdexcept>
#include <iterator>
#include <cctype>
#include <utility>
//Args пакет параметров
template<size_t index, typename... Args>
void print_tuple(std::ostream& output, const std::tuple<Args...>& tuple) {
    if constexpr (index < sizeof...(Args)) { //условие компилятору если не дошли до конца tuple
        output << std::get<index>(tuple);
        
        if (index + 1 < sizeof...(Args)) {
            output << ", ";
        }
        
        print_tuple<index + 1, Args...>(output, tuple);
    }
    // если index >= sizeof...(Args),ничего не делать конец рекурсии
}

//оператор вывода
template<typename... Args>
std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& t) {
    os << "(";
    print_tuple<0,Args...>(os, t);
    os << ")";
    return os;
}

class CsvParseException : public std::runtime_error {
    size_t lineNumber_;
    size_t columnNumber_;
    
public:
    CsvParseException(const std::string& message, size_t line, size_t column)
    : std::runtime_error("Parse error at line " + std::to_string(line) + ", column " + std::to_string(column) + ": " + message),
          lineNumber_(line), columnNumber_(column) {}
    
    size_t Line() const { return lineNumber_; }
    size_t Column() const { return columnNumber_; }
};

class CsvParserConfig {
public:
    char ColumnSeparator = ',';
    char RowSeparator = '\n';
    char quoteChar = '"';
    bool hasHeader = false;
    
    CsvParserConfig() = default;
    
    CsvParserConfig(char colSep, char rowSep = '\n', char quote = '"', bool header = false)
        : ColumnSeparator(colSep)
        , RowSeparator(rowSep)
        , quoteChar(quote)
        , hasHeader(header) 
    { //параметризированный конструктор
        if (colSep == '\0') ColumnSeparator = ',';
        if (rowSep == '\0') RowSeparator = '\n';
        if (quote == '\0') quoteChar = '"';
    }
};

//общий шаблон
template<typename T>
T ConvertFromString(const std::string& str, size_t line, size_t column) {
    std::istringstream iss(str);
    T value;
    if (!(iss >> value)) {
        throw CsvParseException("Cannot convert '" + str + "' to required type", line, column);
    }

    std::string remaining;
    if (iss >> remaining) {
        throw CsvParseException("Cannot convert '" + str + "' to required type", line, column);
    }
    
    return value;
}

//явная специализация для разных типов
template<> 
std::string ConvertFromString<std::string>(const std::string& str, size_t, size_t) {
    return str; // для строки ничего не преобразовываю копирую строку
}

template<>
int ConvertFromString<int>(const std::string& str, size_t line, size_t column) {
    try {
        size_t pos;
        int value = std::stoi(str, &pos);
        for (size_t i = pos; i < str.length(); ++i) {
            if (!std::isspace(static_cast<unsigned char>(str[i]))) {
                throw CsvParseException("Cannot convert '" + str + "' to int", line, column);
            }
        }
        return value;
    } catch (const std::exception&) {
        throw CsvParseException("Cannot convert '" + str + "' to int", line, column);
    }
}

template<>
double ConvertFromString<double>(const std::string& str, size_t line, size_t column) {
    try {
        size_t pos;
        double value = std::stod(str, &pos);
        for (size_t i = pos; i < str.length(); ++i) {
            if (!std::isspace(static_cast<unsigned char>(str[i]))) {
                throw CsvParseException("Cannot convert '" + str + "' to double", line, column);
            }
        }
        return value;
    } catch (const std::exception&) {
        throw CsvParseException("Cannot convert '" + str + "' to double", line, column);
    }
}

template<>
float ConvertFromString<float>(const std::string& str, size_t line, size_t column) {
    try {
        size_t pos;
        float value = std::stof(str, &pos);
        for (size_t i = pos; i < str.length(); ++i) {
            if (!std::isspace(static_cast<unsigned char>(str[i]))) {
                throw CsvParseException("Cannot convert '" + str + "' to float", line, column);
            }
        }
        return value;
    } catch (const std::exception&) {
        throw CsvParseException("Cannot convert '" + str + "' to float", line, column);
    }
}

template<typename... Args>
class CsvParser {
public:
    std::istream* inputStream_;
    CsvParserConfig config_;
    size_t skipLines_;
    size_t currentLine_;
    std::streampos startPos_;
    
    std::string ParseField(const std::string& line, size_t& pos, size_t lineNum, size_t colNum) const {
        std::string output;
        
        if (pos < line.size() && line[pos] == config_.quoteChar) {
            pos++; // пропуск элемента экранирования
            while (pos < line.size()) { // читаю до закр кавычки
                if (line[pos] == config_.quoteChar) {
                    if (pos + 1 < line.size() && line[pos + 1] == config_.quoteChar) {
                        output += config_.quoteChar;
                        pos += 2; // ""
                    } else {
                        pos++;
                        break;
                    }
                } else {
                    output += line[pos++];
                }
            }
        } else { // поле без экранирования
            while (pos < line.size() && line[pos] != config_.ColumnSeparator) {
                output += line[pos++];
            }
        }
        
        return output;
    }
    std::vector<std::string> ParseLine(const std::string& line, size_t lineNum) {
        std::vector<std::string> fields;
        size_t pos = 0;
        size_t colNum = 1;

        while (pos < line.size()) {
            std::string field = ParseField(line, pos, lineNum, colNum);
            fields.push_back(field);

            if (pos < line.size()) {
                if (line[pos] == config_.ColumnSeparator) {
                    pos++; //убираем разделитель
                    colNum++;
                } else {
                    break;
                }
            }
        }

        if (fields.size() < sizeof...(Args)) {
            throw CsvParseException("Expected at least " + std::to_string(sizeof...(Args)) + " columns, got " + std::to_string(fields.size()),lineNum, 1);
        }
        
        return fields;
    }
    
    std::tuple<Args...> MakeTupleFromStrings(const std::vector<std::string>& values, size_t lineNum) {
        return MakeTuple(values, lineNum, std::index_sequence_for<Args...>{});
    }
    
    template<size_t... Indices> //послед индексов для вспомогательно функции
    std::tuple<Args...> MakeTuple(const std::vector<std::string>& values, size_t lineNum, std::index_sequence<Indices...>) {
        return std::make_tuple(
            ConvertFromString<Args>(values[Indices], lineNum, Indices + 1)...//раскрытие пакетов 
            //сопоставление инлекса с типом параметра Args
        );
    }
    
    class Iterator {
    private:
        CsvParser* parser_;
        std::tuple<Args...> currentValue_;
        bool isEnd_;

        void ReadNext() {
            if (isEnd_ || !parser_) return;
            
            std::string line;
            while (true) {
                if (!std::getline(*parser_->inputStream_, line, parser_->config_.RowSeparator)) {
                    isEnd_ = true;
                    return;
                }
                
                parser_->currentLine_++;
                if (line.empty() && parser_->inputStream_->eof()) {
                    continue;
                }
                
                try {
                    auto values = parser_->ParseLine(line, parser_->currentLine_);
                    currentValue_ = parser_->MakeTupleFromStrings(values, parser_->currentLine_);
                    break;
                } catch (const CsvParseException& e) {
                    throw;
                }
            }
        }

    public:
        //конструктор для end итератора
        Iterator() : parser_(nullptr), isEnd_(true) {}

        //конструктор для begin итератора
        Iterator(CsvParser* parser, bool isEnd = false) 
            : parser_(parser), isEnd_(isEnd) {
            if (!isEnd && parser_) {
                ReadNext();
            }
        }
        const std::tuple<Args...>& operator*() const { return currentValue_; }
        Iterator& operator++() {
            ReadNext();
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            if (isEnd_ != other.isEnd_) return true;
            if (isEnd_) return false;
            return parser_ != other.parser_;
        }
    };
    
    CsvParser(std::istream& input, const CsvParserConfig& config = CsvParserConfig())
        : inputStream_(&input)
        , config_(config)
        , currentLine_(0) {
        
        size_t linesToSkip = config_.hasHeader ? 1 : 0;
        skipLines_ = linesToSkip;
        
        startPos_ = inputStream_->tellg();
        
        std::string line;
        for (size_t i = 0; i < skipLines_; ++i) {
            if (!std::getline(*inputStream_, line, config_.RowSeparator)) {
                break;
            }
            currentLine_++;
        }
    }
    
    Iterator begin() {
        inputStream_->clear(); //сброс флага EOF
        inputStream_->seekg(startPos_);
        currentLine_ = 0;
        
        std::string line;
        for (size_t i = 0; i < skipLines_; ++i) {
            if (!std::getline(*inputStream_, line, config_.RowSeparator)) {
                return end();
            }
            currentLine_++;
        }
        
        return Iterator(this, false);
    }
    
    Iterator end() {
        return Iterator();
    }
};

#endif