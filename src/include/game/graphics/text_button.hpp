#pragma once

#include "game/graphics/button.hpp"

class text_button : public button {
    public:
        text_button(std::string name, al::align_horizontal al_x, al::align_vertical al_y, int _x_offset, int _y_offset) : 
        button(name, false, al_x, al_y, _x_offset, _y_offset) {
            action = "none";
            anim.animated = false;
            anim.getSprite();
        }
};