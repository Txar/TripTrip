#pragma once

#include "SFML/Graphics.hpp"
#include "game/graphics/animator.hpp"
#include "game/io/debug.hpp"
#include "game/world/entity.hpp"

class block {
    public:
        bool visible = true, solid = true, kills = false, animated_collider = false;
        std::string name;
        animator anim;

        block(std::string _name = "none") : anim(_name) {
            name = _name;
            if (name == "none") {
                visible = false;
                solid = false;
            }
            anim.setScaling(wrld::SCALING, wrld::SCALING);

            collider = sf::IntRect(0, 0, wrld::BLOCK_SIZE, wrld::BLOCK_SIZE);
        }

        void update(float delta_time) {
            if (visible) anim.update(delta_time);
            if (animated_collider) {
                if ((int)animatedColliders.size() > anim.frame) {
                    collider = animatedColliders.at(anim.frame);
                } else {
                    print("Animated block (" + name + ") lacks colliders!");
                }
            }
        }

        sf::Sprite draw() {
            return anim.getSprite();
        }

        std::vector<sf::IntRect> animatedColliders;
        sf::IntRect collider;
};