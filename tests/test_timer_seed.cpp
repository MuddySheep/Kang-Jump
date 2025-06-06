#include "Timer.h"
#include <iostream>
int main() {
    Timer::Init();
    uint32_t seed = Timer::getSeed32();
    uint32_t pid = Timer::getPID();
    std::cout << seed << ' ' << pid << '\n';
    return seed == 0; // fail if seed is 0
}
