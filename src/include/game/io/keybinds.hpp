#pragma once

#include "game/online/event_packet.hpp"
#include "SFML/Graphics.hpp"

namespace kb {

std::map<sf::Keyboard::Key, std::string> binds;

bool bind_exists(sf::Keyboard::Key k) {
    return (binds.find(k) != binds.end());
}

bool new_bind(sf::Keyboard::Key k, srvr::event *e) {
    if (bind_exists(k)) return false;
    binds.insert(std::pair<sf::Keyboard::Key, std::string>(k, e->type));
    return true;
}

bool new_bind(sf::Keyboard::Key k, std::string s) {
    if (bind_exists(k)) return false;
    binds.insert(std::pair<sf::Keyboard::Key, std::string>(k, s));
    return true;
}

bool remove_bind(sf::Keyboard::Key k) {
    if (!bind_exists(k)) return false;
    binds.erase(k);
    return true;
}

}