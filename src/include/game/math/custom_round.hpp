#pragma once

#include <math.h>
#include "game/world/block.hpp"

int customRound(int x, int base = wrld::BLOCK_SIZE) {
    return int(base * ceil(float(x) / base) - base);
}