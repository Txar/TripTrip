#pragma once

#include "block.hpp"

class animated_block : public block {
    public:
        animated_block(std::string name, int frames, float speed = 1.0, bool pingpong = false) : block(name) {
            anim = animator(name, (int) wrld::BLOCK_SIZE / wrld::SCALING, (int) wrld::BLOCK_SIZE / wrld::SCALING, frames, speed, pingpong);
            anim.setScaling(wrld::SCALING, wrld::SCALING);
        }
};