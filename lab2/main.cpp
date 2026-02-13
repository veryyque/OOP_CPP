#include "LifeParser.h"
#include "Field.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char* argv[]) {
    try {
        string filename;
        if (argc > 1) {
            filename = argv[1];
        }
        
        int birth[10], survive[10];
        int birthCount = 0, surviveCount = 0;
        
        LifeFile lifeFile = readLifeFile(filename, birth, birthCount, survive, surviveCount);
        Field field;

        cout << "Birth: ";
        for (int i = 0; i < birthCount; i++) {
            cout << birth[i] << " ";
        }
        cout << "| Survive: ";
        for (int i = 0; i < surviveCount; i++) {
            cout << survive[i] << " ";
        }
        cout << endl;

        for (int i = 0; i < lifeFile.liveCellsCount; i++) {
            field.setCellAlive(lifeFile.liveCells[i][0], lifeFile.liveCells[i][1]);
        }
        
        if (lifeFile.liveCellsCount == 0) {
            lifeFile.universeName = "Glider";
            field.setCellAlive(1, 0);
            field.setCellAlive(2, 1);
            field.setCellAlive(0, 2);
            field.setCellAlive(1, 2);
            field.setCellAlive(2, 2);
        }

        cout << "Вселенная: " << lifeFile.universeName << endl;
        cout << "Правило: " << lifeFile.rule << endl;
        cout << "Итерация: 0" << endl;
        field.print();

        int iteration = 0;
        string command;
        while (true) {
            cout << "Команда (tick <n=1>, dump <file>, exit, help): ";
            getline(cin, command);

            istringstream iss(command);
            string cmd;
            iss >> cmd; //считывание из строки как из потока

            if (cmd == "exit") {
                break;
            } else if (cmd == "help") {
                cout << "tick <n=1> - n итераций и print" << endl;
                cout << "dump <file> - сохранить" << endl;
                cout << "exit - выход" << endl;
                cout << "help - это" << endl;
            } else if (cmd == "tick" || cmd == "t") {
                int n = 1;
                iss >> n;
                if (n <= 0) n = 1; //ввод без n
                for (int i = 0; i < n; i++) {
                    field.update(birth, birthCount, survive, surviveCount);
                    iteration++;
                }
                cout << "Вселенная: " << lifeFile.universeName << endl;
                cout << "Правило: " << lifeFile.rule << endl;
                cout << "Итерация: " << iteration << endl;
                field.print();
            } else if (cmd == "dump") {
                string outFile;
                iss >> outFile;
                if (outFile.empty()) {
                    cout << "Укажи файл!" << endl;
                } else {
                    field.saveToFile(outFile, lifeFile.universeName, lifeFile.rule);
                    cout << "Сохранено в " << outFile << endl;
                }
            } else {
                cout << "Неизвестная команда. Введите 'help' для справки." << endl;
            }
        }

    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}