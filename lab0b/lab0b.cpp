#include <cctype>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        return 1;
    }

    string input = argv[1];
    string output = argv[2];

    ifstream inFile(input);
    if (!inFile.is_open()) {
        return 1;
    }

    list<string> lines;
    string line;

    while (getline(inFile, line)) {
        lines.push_back(line);
    }
    inFile.close();

    map<string, int> WordCount;
    int wordsNum = 0;

    for (list<string>::const_iterator it = lines.begin(); it != lines.end();
         ++it) {
        const string& currentLine =
            *it;  // разыменование итератора + создаю конст ссылку на строку
        string word;

        for (size_t i = 0; i < currentLine.length(); i++) {
            char c = currentLine[i];
            if (isalpha(c)) {  // если буква
                word += c;
            } else {
                if (!word.empty()) {
                    WordCount[word]++;
                    wordsNum++;
                    word = "";
                }
            }
        }

        if (!word.empty()) {
            WordCount[word]++;
            wordsNum++;
        }
    }

    multimap<int, string, greater<int> > //по убыванию
        sortedByCount;  // мультимэп чтобы слова поменять по ключу и
                        // отсортировать сами ключик
    for (map<string, int>::const_iterator it = WordCount.begin();
         it != WordCount.end(); ++it) {
        sortedByCount.insert(make_pair(it->second, it->first));
    }

    ofstream outFile(output);
    if (!outFile.is_open()) {
        return 1;
    }

    for (multimap<int, string>::const_iterator it = sortedByCount.begin();
         it != sortedByCount.end(); ++it) {
        float percentage = (static_cast<float>(it->first) / wordsNum) * 100.0;
        outFile << "\"" << it->second << "\"," << it->first << "," << percentage
                << endl;
    }

    outFile.close();

    return 0;
}
