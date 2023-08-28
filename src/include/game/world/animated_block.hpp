#include "block.hpp"

#pragma once

class animated_block : public block {
    public:
        animated_block(std::string name, int frames, float speed = 1.0, bool pingpong = false) : block(name) {
            anim = animator(name, wrld::BLOCK_SIZE, wrld::BLOCK_SIZE, frames, speed, pingpong);
        }
};