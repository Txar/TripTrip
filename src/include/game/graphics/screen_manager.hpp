#include "SFML/Graphics.hpp"
#include "game/io/events.hpp"
#include "game/world/entity.hpp"
#include "game/world/block.hpp"
#include "game/io/debug.hpp"
#include <iostream>

#pragma once

class screen_manager {
    private:
        int screen_height, screen_width;
        sf::Font font;
        sf::View camera;

    public:
        sf::RenderWindow window;

        screen_manager(std::string title, int _screen_width, int _screen_height) : window(sf::VideoMode(_screen_width, _screen_height), title) {
            screen_width = _screen_width;
            screen_height = _screen_height;
            camera.setSize({float(screen_width), float(screen_height)});
            window.setFramerateLimit(60);

            if (!font.loadFromFile("assets/Roboto-Regular.ttf")){
                std::cout << "Font missing!" << std::endl;
            }
        }

        ~screen_manager() {
            window.close();
        }

        void drawEntities(std::vector<entity*> *entities, bool draw_colliders = false) {
            sf::RenderTexture entity_buffer;
            entity_buffer.create(screen_width, screen_height);
            entity_buffer.clear(sf::Color(0, 0, 0, 0));
            for (entity * i : *entities) {
                sf::Sprite s = i->draw();
                s.setPosition(sf::Vector2f(s.getPosition().x - float(wrld::camera_x + 64 - screen_width/2), 
                                           s.getPosition().y - float(wrld::camera_y + 64 - screen_height/2)));
                entity_buffer.draw(s);
                if (draw_colliders) {
                    for (int j = 0; j < int(i->colliders.size()); j++){
                        sf::IntRect c = i->colliders.at(j).first;
                        c.left += i->x;
                        c.top += i->y;
                        sf::RectangleShape s({float(c.width), float(c.height)});
                        s.setPosition(c.left - float(wrld::camera_x + 64 - screen_width/2), 
                                      c.top - float(wrld::camera_y + 64 - screen_height/2));
                        s.setFillColor(sf::Color(0, 0, 0, 0));
                        s.setOutlineColor(sf::Color(255, 0, 0));
                        s.setOutlineThickness(1.0);
                        entity_buffer.draw(s);
                    }
                }
            }
            entity_buffer.display();
            sf::Sprite s(entity_buffer.getTexture());
            s.setPosition(float(wrld::camera_x + 64 - screen_width/2), float(wrld::camera_y + 64 - screen_height/2));
            window.draw(s);
        }

        void draw_blocks(block ***blocks) {
            sf::RenderTexture blocks_buffer;
            blocks_buffer.create(screen_width, screen_height);
            blocks_buffer.clear(sf::Color(0, 0, 0, 0));
            for (int i = (wrld::camera_x - screen_width/2 - 1)/wrld::BLOCK_SIZE; i < (wrld::camera_x + screen_width/2 + 1)/wrld::BLOCK_SIZE + 2; i++) { 
                if (i < 0 || i >= wrld::WORLD_WIDTH) continue;
                for (int j = (wrld::camera_y - screen_height/2)/wrld::BLOCK_SIZE; j < (wrld::camera_y + screen_height/2)/wrld::BLOCK_SIZE + 2; j++) {
                    if (j < 0 || j > wrld::WORLD_HEIGHT - 1 || !blocks[i][j]->visible) continue;
                    if (j < 0 || j > wrld::WORLD_HEIGHT - 1 || !blocks[i][j]->visible) continue;
                    sf::Sprite s = (*blocks[i][j]).draw();
                    s.setPosition(sf::Vector2f(i * wrld::BLOCK_SIZE - float(wrld::camera_x + 64 - screen_width/2),
                                               j * wrld::BLOCK_SIZE - float(wrld::camera_y + 64 - screen_height/2)));
                    blocks_buffer.draw(s);
                }
            }
            blocks_buffer.display();
            sf::Sprite s(blocks_buffer.getTexture());
            s.setPosition(float(wrld::camera_x + 64 - screen_width/2), float(wrld::camera_y + 64 - screen_height/2));
            window.draw(s);
        }

        void clear() {
            window.clear(sf::Color(0, 0, 0));
        }

        bool update() {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) window.setFramerateLimit(60);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) window.setFramerateLimit(200);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) window.setFramerateLimit(30);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) window.setFramerateLimit(45);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) window.setFramerateLimit(50);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) window.setFramerateLimit(55);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) window.setFramerateLimit(0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) return false;

            sf::Text text(std::to_string(wrld::fps) + " FPS\n" + de.console, font, 16);

            text.setFillColor(sf::Color(255, 0, 0));
            text.setPosition(sf::Vector2f(camera.getCenter().x - screen_width / 2, camera.getCenter().y - screen_height / 2));
            window.draw(text);

            camera.setCenter({float(wrld::camera_x + 64), float(wrld::camera_y + 64)});
            //camera.reset({float(wrld::camera_x + 64 - screen_width/2), float(wrld::camera_y + 64 - screen_height/2), float(screen_width), float(screen_height)}); // wrld::camera_center, {float(screen_width), float(screen_height)}
            window.setView(camera);
            em::update(&window);
            for (sf::Event i : em::events) {
                if (i.type == sf::Event::EventType::Closed) {
                    return false;
                }
            }
            window.display();
            return true;
        }
};