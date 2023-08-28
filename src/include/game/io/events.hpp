#pragma once
#include "SFML/Graphics.hpp"
#include "game/online/event_packet.hpp"
#include "keybinds.hpp"

namespace em { //so called "event manager"
    std::vector<sf::Event> events;
    std::string player_name;

    void update(sf::RenderWindow * window) {
        sf::Event event;
        events.clear();
        while ((*window).pollEvent(event)){
            events.push_back(event);
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