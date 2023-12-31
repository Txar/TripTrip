#pragma once

#include "SFML/Graphics.hpp"
#include "game/graphics/animator.hpp"
#include "game/io/debug.hpp"
#include "game/world/entity.hpp"

class block {
    public:
        bool visible = true, solid = true, kills = false, animated_collider = false, draw_shadow = false, is_background = false, is_finish = false;
        std::string name;
        animator anim;

        block(std::string _name = "none", bool _draw_shadow = false) : anim(_name) {
            name = _name;
            draw_shadow = _draw_shadow;
            if (name == "none") {
                visible = false;
                solid = false;
            }
            is_background = !solid;
            anim.setScaling(wrld::SCALING, wrld::SCALING);

            collider = sf::IntRect(0, 0, wrld::BLOCK_SIZE, wrld::BLOCK_SIZE);
        }

        void update(float delta_time) {
            if (visible) anim.update(delta_time);
            if (animated_collider) {
                if ((int)animatedColliders.size() > anim.frame) {
                    collider = animatedColliders.at(anim.frame);
                    collider.left *= wrld::SCALING;
                    collider.width *= wrld::SCALING;
                    collider.top *= wrld::SCALING;
                    collider.height *= wrld::SCALING;
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