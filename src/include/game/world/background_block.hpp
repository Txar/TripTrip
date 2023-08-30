#pragma once

#include "block.hpp"

class background_block : public block {
    public:
        background_block(std::string name) : block(name) {
            solid = false;
        }
};