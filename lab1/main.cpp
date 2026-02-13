#include <iostream>
#include <string>
#include "BitArray.h"

int main() {
    BitArray ba(8, 0xAAUL);
    std::cout << "BitArray: " << ba.to_string() << std::endl;
    std::cout << "Size: " << ba.size() << std::endl;
    return 0;
}