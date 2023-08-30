#pragma once

#include "SFML/Graphics.hpp"
#include "game/io/events.hpp"
#include "game/world/entity.hpp"
#include "game/world/block.hpp"
#include "game/io/debug.hpp"
#include "game/graphics/ui.hpp"
#include <iostream>

class screen_manager {
    private:
        sf::Font font;
        sf::View camera;
        //sf::Color background_color = {119, 157, 206};
        sf::Color background_color = {90, 90, 90};
        sf::Color background_filter = {0, 0, 0, 160};

        sf::Color pause_color = {60, 60, 60, 160};

        sf::Color tin ={0, 0, 0, 60};
        //sf::Color tin = {0, 0, 0, 0};

        std::vector<sf::Sprite> background_sprites;

        const float BACKGROUND_SCALING = 16.0;

    public:
        int screen_height, screen_width;

        sf::RenderWindow window;

        screen_manager(std::string title, int _screen_width, int _screen_height) : window(sf::VideoMode(_screen_width, _screen_height), title) {
            screen_width = _screen_width;
            screen_height = _screen_height;
            camera.setSize({float(screen_width), float(screen_height)});
            window.setFramerateLimit(60);

            //texture_mgr.onload_scaling = BACKGROUND_SCALING;
            background_sprites = {texture_mgr.get_sprite("background_0"), texture_mgr.get_sprite("background_1"), texture_mgr.get_sprite("background_2")};
            //texture_mgr.onload_scaling = 1.0;

            if (!font.loadFromFile("assets/Roboto-Regular.ttf")){
                std::cout << "Font missing!" << std::endl;
            }

            if (!wrld::font.loadFromFile("assets/joystix_monospace.otf")){
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
                s.setPosition(sf::Vector2f(s.getPosition().x - float(wrld::camera_x - screen_width/2), 
                                           s.getPosition().y - float(wrld::camera_y - screen_height/2)));
                entity_buffer.draw(s);
                if (draw_colliders) {
                    for (int j = 0; j < int(i->colliders.size()); j++){
                        sf::IntRect c = i->colliders.at(j).first;
                        c.left += i->x;
                        c.top += i->y;
                        sf::RectangleShape s({float(c.width), float(c.height)});
                        s.setPosition(c.left - float(wrld::camera_x - screen_width/2), 
                                      c.top - float(wrld::camera_y - screen_height/2));
                        s.setFillColor(sf::Color(0, 0, 0, 0));
                        s.setOutlineColor(sf::Color(255, 0, 0));
                        s.setOutlineThickness(1.0);
                        entity_buffer.draw(s);
                    }
                }
            }
            entity_buffer.display();
            sf::Sprite s(entity_buffer.getTexture());
            s.setPosition(float(wrld::camera_x - screen_width/2), float(wrld::camera_y - screen_height/2));
            window.draw(s);
        }

        void drawBlocks(block ***blocks) {
            sf::RenderTexture blocks_buffer;
            sf::RenderTexture background_buffer;
            sf::RenderTexture shadows_buffer;

            sf::Sprite shadow_sprite = texture_mgr.get_sprite("shadow");
            shadow_sprite.setScale(4.0, 4.0);

            blocks_buffer.create(screen_width, screen_height);
            blocks_buffer.clear(sf::Color(0, 0, 0, 0));
            background_buffer.create(screen_width, screen_height);
            background_buffer.clear(sf::Color(0, 0, 0, 0));
            shadows_buffer.create(screen_width, screen_height);
            shadows_buffer.clear(sf::Color(0, 0, 0, 0));
            for (int i = (wrld::camera_x - screen_width/2 - 1)/wrld::BLOCK_SIZE - 2; i < (wrld::camera_x + screen_width/2 + 1)/wrld::BLOCK_SIZE + 2; i++) { 
                if (i < 0 || i >= wrld::WORLD_WIDTH) continue;
                for (int j = (wrld::camera_y - screen_height/2)/wrld::BLOCK_SIZE; j < (wrld::camera_y + screen_height/2)/wrld::BLOCK_SIZE + 2; j++) {
                    if (j < 0 || j > wrld::WORLD_HEIGHT - 1 || !blocks[i][j]->visible) continue;
                    sf::Sprite s = (*blocks[i][j]).draw();
                    s.setPosition(sf::Vector2f(i * wrld::BLOCK_SIZE - float(wrld::camera_x - screen_width/2),
                                               j * wrld::BLOCK_SIZE - float(wrld::camera_y - screen_height/2)));
                    if (blocks[i][j]->solid && !blocks[i][j]->is_background) {
                        blocks_buffer.draw(s);
                    } else {
                        background_buffer.draw(s);
                    }

                    if (blocks[i][j]->draw_shadow) {
                        shadow_sprite.setPosition(s.getPosition().x - 12, s.getPosition().y - 12);
                        shadows_buffer.draw(shadow_sprite);
                    }
                }
            }
            background_buffer.display();
            blocks_buffer.display();
            shadows_buffer.display();

            sf::Sprite s(background_buffer.getTexture());
            s.setPosition(float(wrld::camera_x - screen_width/2), float(wrld::camera_y - screen_height/2));
            window.draw(s);

            s.setTexture(shadows_buffer.getTexture());
            window.draw(s);

            s.setTexture(blocks_buffer.getTexture());
            window.draw(s);
        }

        void drawBackground() {
            sf::Vector2f c = camera.getCenter();
            for (int i = 0; i < (int) background_sprites.size(); i++) {
                sf::FloatRect r = {c.x - screen_width/2, wrld::BLOCK_SIZE * wrld::WORLD_HEIGHT - BACKGROUND_SCALING * 128, (float) screen_width + wrld::BLOCK_SIZE * 2, (float) screen_height};
                sf::Sprite *s = &background_sprites.at(i);
                s->setScale(BACKGROUND_SCALING, BACKGROUND_SCALING);
                r.left = r.left - r.left * sqrt(sqrt(sqrt((i * i) + 1))) - wrld::BLOCK_SIZE;
                s->setPosition((float)r.left, (float)r.top);
                //s->setTextureRect(sf::IntRect(r));
                window.draw(*s);
            }
            sf::RectangleShape filter({(float) screen_width + wrld::BLOCK_SIZE * 2, (float) screen_height});
            filter.setPosition(c.x - screen_width/2, c.y - screen_height/2);
            filter.setFillColor(background_filter);
            window.draw(filter);
        }

        void clear() {
            window.clear(background_color);
        }

        void moveCamera() {
            camera.setCenter({float(wrld::camera_x), float(wrld::camera_y)});
        }

        void drawConsole() {
            sf::Text text(std::to_string(wrld::fps) + " FPS\n" + de.console, font, 16);

            text.setFillColor(sf::Color(255, 0, 0));
            text.setPosition(sf::Vector2f(camera.getCenter().x - screen_width / 2, camera.getCenter().y - screen_height / 2));
            window.draw(text);
        }

        void drawUI(ui &ui_mgr, bool draw_console = true) {
            button t;
            button *b = &t;

            sf::Vector2f cam = {(float)wrld::camera_x - screen_width / 2, (float)wrld::camera_y - screen_height / 2};

            sf::RectangleShape bg({(float)screen_width, (float) screen_height});
            bg.setFillColor(tin);
            bg.setPosition(cam.x, cam.y);
            window.draw(bg);

            if (wrld::paused) {
                sf::RectangleShape bg({(float)screen_width, (float) screen_height});
                bg.setFillColor(pause_color);
                bg.setPosition(cam.x, cam.y);
                window.draw(bg);
            }

            while (ui_mgr.iter(b)) {
                if (ui_mgr.scene != "none" && b->scene != "all" && b->scene != ui_mgr.scene) continue;
                sf::Sprite s = b->anim.getSprite();
                s.setPosition(b->x + cam.x, b->y + cam.y);
                window.draw(s);
            }

            if (draw_console) drawConsole();
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

            //camera.reset({float(wrld::camera_x + 64 - screen_width/2), float(wrld::camera_y + 64 - screen_height/2), float(screen_width), float(screen_height)}); // wrld::camera_center, {float(screen_width), float(screen_height)}
            
            em::window_width = screen_width;
            em::window_height = screen_height;

            window.setView(camera);
            em::update(&window);
            for (sf::Event i : em::events) {
                if (i.type == sf::Event::Resized) {
                    camera.setSize(i.size.width, i.size.height);
                    screen_width = i.size.width;
                    screen_height = i.size.height;
                }
                if (i.type == sf::Event::EventType::Closed) {
                    return false;
                }
            }

            window.display();
            return true;
        }
};