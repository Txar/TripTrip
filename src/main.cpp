#include "game/game.hpp"
#include <iostream>
#include "game/math/random.hpp"
#include "SFML/Network.hpp"
#include "game/util/list_contains_string.hpp"
#include <random>

int main(){
    //socket testing

    /*int mode;
    std::cin >> mode;
    sf::UdpSocket socket;
    sf::Packet p;
    socket.setBlocking(false);
    if (mode == 0) {
        p << "haha ale string se leci";
        char data[100] = "haha ale string se leci";
        sf::IpAddress recipient = "192.168.1.88";
        unsigned short port = 22355;
        if (socket.send(p, recipient, port) != sf::Socket::Done) {
            std::cout << "Send FAIL" << std::endl;
        }
        else {
            std::cout << "Succesfuly sent \"" << data << "\"" << std::endl;
        }
    }
    else {
        char data[100];
        std::size_t received;
        sf::IpAddress sender;
        unsigned short port;
        if (socket.bind(22355) != sf::Socket::Done) {
            std::cout << "Bind FAIL" << std::endl;
        }

        while (socket.receive(p, sender, port) != sf::Socket::Done) {
            std::cout << "Receive FAIL ";// << std::endl;
        }
        std::string s;
        p >> s;
        std::cout << "Received \"" << s << "\" on port " << port << std::endl;
        //std::cout << "Data: " << data << std::endl;
    }
    */

    game TripTrip(true);
    return TripTrip.main_loop();
}