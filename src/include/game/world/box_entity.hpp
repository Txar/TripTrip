#include "block.hpp"
#include "entity.hpp"
#include "game/io/events.hpp"
#include <cmath>
#include <iostream>

#pragma once

class box_entity : public entity {
    protected:
        void applyMovement(float delta_time) {
            if (velocity.x != 0 && fabs(velocity.x * delta_time) < 0.02) velocity.x = 0.0;
            if (velocity.x < 0.0) {
                if (bottomCollider && applyFriction) applyForce({wrld::FRICTION_COEFFICIENT_NORMAL * mass * wrld::G, 0}, delta_time);
                if (!leftCollider)   x += (velocity.x * delta_time) + ((previousVelocity.x - velocity.x) * delta_time * delta_time) / 2;
                else velocity.x = 0;
            }
            if (velocity.x > 0.0) {
                if (bottomCollider && applyFriction) applyForce({-wrld::FRICTION_COEFFICIENT_NORMAL * mass * wrld::G, 0}, delta_time);
                if (!rightCollider)  x += (velocity.x * delta_time) + ((previousVelocity.x - velocity.x) * delta_time * delta_time) / 2;
                else velocity.x = 0;
            }
            if (velocity.y < 0.0) {
                if (!topCollider)    y += (velocity.y * delta_time) + ((previousVelocity.y - velocity.y) * delta_time * delta_time) / 2;
                else velocity.y = 0;
            }
            if (velocity.y > 0.0) {
                if (!bottomCollider) y += (velocity.y * delta_time) + ((previousVelocity.y - velocity.y) * delta_time * delta_time) / 2;
                else velocity.y = 0;
            }
        }

    public:
        void actOnEvents() {
            
        }

        bool leftCollider, rightCollider, topCollider, bottomCollider;
        bool doApplyMovement = false;

        box_entity(entity &e) : entity(e) {
            
        }

        box_entity(std::string name, int _width, int _height, std::string _type = "boxEntity") : entity {name, _width, _height, 2.0, _type} {
            colliders = {
                {{0, 8, 8, height - 16}, false}, //left
                {{width - 8, 8, 8, height - 16}, false}, //right
                {{8, 0, width - 16, 8}, false}, //top
                {{8, height, width - 16, 8}, false} //bottom
            };
        };

        virtual void update(float delta_time) {
            entity::update(delta_time);

            leftCollider = colliders.at(0).second;
            rightCollider = colliders.at(1).second;
            topCollider = colliders.at(2).second;
            bottomCollider = colliders.at(3).second;

            previousVelocity = velocity;
            if (applyGravity) applyForce({0, wrld::G * mass}, delta_time);
            if (doApplyMovement) applyMovement(delta_time);
        }
};