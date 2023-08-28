#include "SFML/Graphics.hpp"
#include "game/graphics/animator.hpp"
#include "block.hpp"

#pragma once

namespace wrld {
    float camera_x = 0;
    float camera_y = 0;
    int fps;
    sf::Vector2f camera_center;

    const float G = 1.0;
    const float FRICTION_COEFFICIENT_NORMAL = 0.1;

    const float FORCE_MULTIPLIER = wrld::BLOCK_SIZE * 16;
}

class entity {
    public:
        void actOnEvents() {
            
        }

        entity(std::string _name, int _width, int _height, float weight = 1.0, std::string _type = "entity") {
            name = _name;
            width = _width;
            height = _height;
            x = 0;
            y = 0;
            mass = weight;
            type = _type;
        }

        ~entity() {

        }

        virtual void update(float delta_time) {
            hasChanged = false;
            for (int i = 0; i < int(animators.size()); i++) hasChanged += animators.at(i).update(delta_time);
            //std::cout << velocity.x * delta_time << std::endl;
        }

        void applyForce(sf::Vector2f force, float delta_time) {
            velocity += sf::Vector2f(force.x * wrld::FORCE_MULTIPLIER * delta_time, force.y * wrld::FORCE_MULTIPLIER * delta_time);
        }

        void summon(float _x, float _y) { //, int _width, int _height) {
            x = _x;
            y = _y;
            alive = true;
            //width = _width;
            //height = _height;
        }

        void kill() {
            alive = false;
            x = 0;
            y = 0;
        }

        void after() { //called after update

        }

        sf::Sprite draw() {
            if (hasChanged || !textureCreated) {
                sf::RenderTexture rt;
                rt.create(width, height);
                rt.clear(sf::Color(0, 0, 0, 0));
                for (animator i : animators) {
                    rt.draw(i.getSprite());
                }
                rt.display();
                t = rt.getTexture();
                textureCreated = true;
            }
            sf::Sprite s(t);
            s.setPosition({x, y});
            return s;
        }

        sf::Vector2f previousVelocity = {0.0, 0.0};
        sf::Vector2f velocity = {0.0, 0.0};
        std::string name;
        int width, height;
        float x, y;
        float mass;
        bool alive;
        bool grounded = false;
        bool hasChanged = false;
        bool textureCreated = false;
        std::vector<std::pair<sf::IntRect, bool>> colliders; //the bool is saying whether it's colliding with something or not
        std::vector<std::pair<sf::IntRect, bool>> damageColliders; //same here
        bool applyGravity = true, applyFriction = true;
        std::string type;
        std::vector<animator> animators;

    protected:
        sf::Texture t;
};