#pragma once

#include "game/content/sound_engine.hpp"
#include "box_entity.hpp"
#include "game/io/events.hpp"
#include "game/math/cap.hpp"
#include <iostream>
#include <cmath>
#include "game/online/event_packet.hpp"


class player_entity : public box_entity {
    protected:

    public:
        virtual void actOnEvents() {
            if (alive) {
                srvr::event e;
                pressingRight = false, pressingLeft = false, pressingJump = false;
                while (srvr::eventPool.iter(&e)) {
                    if (e.player != name) {
                        //std::cout << e.player << " " << name << "\n";
                        continue;
                    } 
                    if (e.type == "jump") pressingJump = true;
                    if (e.type == "move_left") pressingLeft = true;
                    if (e.type == "move_right") pressingRight = true;
                }
            }
        }
        
        bool holdingJump = false, justJumped = false, running = false;
        bool pressingRight = false, pressingLeft = false, pressingJump = false;
        bool facingLeft = false;
        float jumpLoad = 1.0;
        float maxJumpTime = 0.08;
        float jumpVelocity = 16.0;
        float maxRunningVelocity = 1.0;
        float runningAcceleration = 1.0;
        
        float coyote_time = 0.1;
        float time_since_standing = 0.0;

        float grace = false;

        float stepSoundDelay = 0.5;
        float stepSoundCounter = stepSoundDelay;

        player_entity(entity &e) : box_entity{e} {
            alive = true;
        }

        player_entity(std::string name, int _width, int _height, std::string _type = "playerEntity") : box_entity {name, _width, _height, _type} {
            animators.push_back(animator("none", 32, 32, 6));
            colliders = {
                {{24, 40, 8, height - 48}, false}, //left
                {{width - 36, 40, 8, height - 48}, false}, //right
                {{44, 24, width - 88, 8}, false}, //top
                {{44, height, width - 88, 24}, false} //bottom
            };

            damageColliders = {
                {{32, 32, width - 64, height - 64}, false}
            };

            mass = 3.0;
            //applyFriction = false;
            animators.at(0).setScaling(4.0, 4.0);
            animators.at(0).name = "tick-stand";
            wrld::sound_mgr.load("walk");
            wrld::sound_mgr.load("death");
            wrld::sound_mgr.load("jump");
        };

        void kill() {
            alive = false;
            wrld::death_counter++;
            wrld::sound_mgr.play("death");
            wrld::player_is_alive = false;
        }

        virtual void update(float delta_time) {
            box_entity::update(delta_time);
            time_since_standing += delta_time;
            grace = false;
            if (!bottomCollider && time_since_standing <= coyote_time) {
                grace = true;
            } else if (bottomCollider) time_since_standing = 0;

            //std::cout << velocity.x << std::endl;
            if (abs(velocity.x) > 0.0) {
                animators.at(0).speed = capAt(std::fabs(velocity.x) * delta_time * 16, 8.0);
                //if (!bottomCollider) animators.at(0).speed = 0;
                running = true;
                if (velocity.x > 0.0) facingLeft = false;
                else facingLeft = true;
            }
            else {
                running = false;
            }

            animators.at(0).flipSprite = facingLeft;
            if (!bottomCollider) {
                if (animators.at(0).name == "tick-stand" || animators.at(0).name == "tick-run") {
                    wrld::sound_mgr.play("jump");
                }
                animators.at(0).name = "tick-fall";
                animators.at(0).animated = false;
                animators.at(0).frame = 0;
                hasChanged = true;
            }
            else if (!running) {
                if (animators.at(0).name == "tick-fall") {
                    wrld::sound_mgr.play("jump");
                }
                animators.at(0).name = "tick-stand";
                animators.at(0).animated = false;
                animators.at(0).frame = 0;
                hasChanged = true;
            }
            else {
                if (animators.at(0).name == "tick-fall") {
                    wrld::sound_mgr.play("jump");
                }
                stepSoundCounter -= delta_time;
                if (hasChanged && (animators.at(0).frame == 0 || animators.at(0).frame == 3)) {
                    wrld::sound_mgr.play("walk");
                }
                animators.at(0).name = "tick-run";
                animators.at(0).animated = true;
                hasChanged = true;
            }

            if (velocity.x < maxRunningVelocity * wrld::FORCE_MULTIPLIER) {
                if (pressingRight) applyForce({runningAcceleration, 0}, delta_time);
            }
            if (-velocity.x < maxRunningVelocity * wrld::FORCE_MULTIPLIER) {
                if (pressingLeft) applyForce({-runningAcceleration, 0}, delta_time);
            }

            if (pressingJump && !justJumped && (bottomCollider || grace)) {
                holdingJump = true;
                justJumped = true;
            }

            if (pressingJump) {
                if (holdingJump && jumpLoad > 0.0) {
                    applyForce({0, -jumpVelocity}, delta_time);
                    jumpLoad -= (1.0 / maxJumpTime) * delta_time;
                }
            }
            else holdingJump = false;
            if (bottomCollider && jumpLoad < 1.0 && !holdingJump) {
                justJumped = false;
                jumpLoad = 1.0;
            }

            if (damageColliders.at(0).second) {
                kill();
            }

            applyMovement(delta_time);
        }
};