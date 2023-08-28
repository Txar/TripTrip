#include "SFML/Graphics.hpp"
#include "game/graphics/animator.hpp"

#pragma once

namespace wrld {
    const int BLOCK_SIZE = 64;

    const int WORLD_WIDTH = 128;
    const int WORLD_HEIGHT = 128;
}

class block {
    public:
        bool visible = true, solid = true;
        std::string name;
        animator anim;

        block(std::string _name = "none") : anim(_name) {
            name = _name;
            if (name == "none") {
                visible = false;
                solid = false;
            }

            collider = sf::IntRect(0, 0, wrld::BLOCK_SIZE, wrld::BLOCK_SIZE);
        }

        void update(float delta_time) {
            if (visible) anim.update(delta_time);
        }

        sf::Sprite draw() {
            return anim.getSprite();
        }

        sf::IntRect collider;
};