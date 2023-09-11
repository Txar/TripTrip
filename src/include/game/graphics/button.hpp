#pragma once

#include "game/graphics/animator.hpp"
#include "game/io/events.hpp"

namespace al {

enum align_horizontal {
    LEFT, CENTER, RIGHT
};

enum align_vertical {
    TOP, MIDDLE, BOTTOM
};

}

namespace wrld {
    const float UI_SCALING = 2.0;
}

class button {
    public:
        animator anim;
        bool pressed = false;
        bool switchable;
        bool visible = true;
        bool freeze = false;

        int level = -1;

        std::string scene = "all";
        std::string action = "none";

        int x, y;

        al::align_horizontal align_x = al::CENTER;
        al::align_vertical align_y = al::MIDDLE;

        int x_offset = 0, y_offset = 0;

        button() : anim("none", 32, 32, 2, 0.0) {};

        button(std::string name, bool _switchable, al::align_horizontal al_x, al::align_vertical al_y, int _x_offset, int _y_offset) :
        anim(name, 32, 32, 2, 0.0) {
            align_x = al_x;
            align_y = al_y;
            switchable = _switchable;
            anim.setScaling(wrld::UI_SCALING, wrld::UI_SCALING);
            x_offset = _x_offset;
            y_offset = _y_offset;
        }

        button(std::string name, bool _switchable, int width = 32, int height = 32, int frames = 2) :
        anim(name, width, height, frames, 0.0) {
            switchable = _switchable;
            anim.setScaling(wrld::UI_SCALING, wrld::UI_SCALING);
        }

        void updatePosition() {
            switch (align_x) {
                case al::LEFT:
                    x = 0;
                    break;
                case al::CENTER:
                    x = (int) (em::window_width - (anim.width * wrld::UI_SCALING)) / 2;
                    break;
                case al::RIGHT:
                    x = em::window_width - (anim.width * wrld::UI_SCALING);
                    break;
            }
            switch (align_y) {
                case al::TOP:
                    y = 0;
                    break;
                case al::MIDDLE:
                    y = (int) (em::window_height - (anim.height * wrld::UI_SCALING)) / 2;
                    break;
                case al::BOTTOM:
                    y = em::window_height - (anim.height * wrld::UI_SCALING);
                    break;
            }
            
            x += x_offset * wrld::UI_SCALING;
            y += y_offset * wrld::UI_SCALING;
        }

        bool isPressed(int mouse_x, int mouse_y) {
            return visible && (mouse_x >= x && mouse_x <= x + (anim.width * wrld::UI_SCALING)) && (mouse_y >= y && mouse_y <= y + (anim.height * wrld::UI_SCALING));
        }

        void update() {
            updatePosition();
            if (!freeze) anim.frame = (int) pressed;
            if (!switchable) pressed = false;
        }

        void press() {
            if (switchable && pressed) pressed = false;
            else pressed = true;
        }
};