#include "SECPK1/Int.h"
#include <iostream>

int main() {
    if (is_little_endian) {
        std::cout << "little-endian\n";
        return 0;
    }
    std::cout << "big-endian\n";
    return 1;
}
