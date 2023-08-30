#pragma once

#include "SFML/Graphics.hpp"
#include "game/online/event_packet.hpp"
#include "keybinds.hpp"

namespace em { //so called "event manager"
    std::vector<sf::Event> events;
    std::string player_name;

    bool mouseHasBeenPressed = false;

    int mouse_x;
    int mouse_y;

    int window_width = 0;
    int window_height = 0;

    bool draw_background = true;

    void update(sf::RenderWindow *window) {
        sf::Vector2i mousepos = sf::Mouse::getPosition(*window);
        mouse_x = mousepos.x;
        mouse_y = mousepos.y;
        mouseHasBeenPressed = false;
        sf::Event event;
        events.clear();
        while (window->pollEvent(event)){
            events.push_back(event);
            if (event.type == sf::Event::MouseButtonPressed) {
                mouseHasBeenPressed = true;
            }
        }
    }

    void updateGlobalEvents(srvr::event_pool *e) {
        e->flush();
        for (auto i : kb::binds) {
            if (sf::Keyboard::isKeyPressed(i.first)) {
                srvr::event ev(player_name, i.second);
                e->dump(&ev);
            }
        }
    }

    void updateGlobalEvents() {
        updateGlobalEvents(&srvr::eventPool);
    }
}