#include <iostream>
#include <math.h>
#include <cstdint>

using namespace std;

template <typename T>
void sanitizeInput(const string output, T &value, const T lowLimit, const T highLimit) {
    while ((cout << output) && (!(cin >> value) || (value < lowLimit || value > highLimit))) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

uint16_t getHighSeed(const uint32_t seed) {
    return seed >> 16;
}

uint32_t LCRNG(uint32_t seed) {
    return 0x41C64E6D * seed + 0x6073;
}

uint16_t getNextHighSeed(const uint32_t seed) {
    return getHighSeed(LCRNG(seed));
}

uint32_t LCRNGR(const uint32_t seed) {
    return 0xEEB9EB65 * seed + 0xA3561A1;
}

void findInitialSeed(uint32_t seed) {
    uint16_t highSeed = getHighSeed(seed);
    unsigned long advances = 0;

    while (highSeed != 0) {
        seed = LCRNGR(seed);
        highSeed = getHighSeed(seed);
        advances++;
    }

    printf("\nInitial Seed: %04X\nAdvances: %lu\n\n------------------------------------------------\n\n", seed, advances);
}

bool findIDsSeed(const uint16_t tid, const uint16_t sid) {
    for (uint32_t seed = sid << 16; seed <= 4294967295; seed++) {
        if (getNextHighSeed(seed) == tid) {
            findInitialSeed(LCRNGR(seed));
            return true;
        }
    }

    return false;
}

int main() {
    while (true) {
        uint16_t tid, sid = 0;

        sanitizeInput<uint16_t>("Insert the wanted TID: ", tid, 0, 65535);
        sanitizeInput<uint16_t>("Insert the wanted SID: ", sid, 0, 65535);

        if (!findIDsSeed(tid, sid)) {
            cout << "\nThe inserted TID/SID combo is not possible\n\n------------------------------------------------\n\n";
        }
    }
}