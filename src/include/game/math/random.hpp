#include <random>
#include <time.h>
#pragma once

namespace wrld {
    int seed = 0;
    int seed_index = 0;
}

void genSeed() {
    srand(time(NULL));
    while (wrld::seed < 1000000000) {
        wrld::seed += rand();
    }
}

int frandint() {
    return 0;
}

int frand() {
    return 0;
}