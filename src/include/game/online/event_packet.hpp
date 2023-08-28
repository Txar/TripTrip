#pragma once
#include <iostream>
#include "SFML/Network.hpp"

namespace srvr {

std::string event_types[] = {
    "jump", "move_left", "move_right", "none"
};

class event {
    public:
        std::string player;
        std::string type;

        event(std::string _player, std::string _type) : player(_player), type(_type) {};

        event() : player("none"), type("none") {};
};

class event_pool {
    public:
        std::vector<event> pool;
        int iterator = 0;

        void dump(event *e) {
            pool.push_back(*e);
        }

        void flush() {
            iterator = 0;
            pool.clear();
        }

        bool isEmpty() {
            return pool.size() == 0;
        }

        bool iter(event *e) {
            if (iterator == int(pool.size())) {
                iterator = 0;
                return false;
            }
            else {
                *e = pool.at(iterator);
                iterator++;
                return true;
            }
        }
};

sf::Packet prepareEventPacket(event_pool *ep);

event_pool eventPool;

}

sf::Packet& operator <<(sf::Packet& packet, srvr::event &e)
{
    return packet << e.player << e.type;
}

sf::Packet& operator >>(sf::Packet& packet, srvr::event &e)
{
    return packet >> e.player >> e.type;
}

sf::Packet& operator <<(sf::Packet& packet, srvr::event_pool &ep)
{
    srvr::event i;
    while (ep.iter(&i)) {
        packet << i;
    }

    return packet;
}

sf::Packet& operator >>(sf::Packet& packet, srvr::event_pool &ep)
{
    srvr::event e;
    while (!packet.endOfPacket()) {
        packet >> e;
        ep.dump(&e);
    }
    return packet;
}


namespace srvr {

sf::Packet prepareEventPacket(srvr::event_pool *ep) {
    sf::Packet p;
    p << (*ep);
    return p;
}

};