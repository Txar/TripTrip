#include <iostream>
#include <cmath>
#include <algorithm>
#include "graphics/screen_manager.hpp"
#include "world/player_entity.hpp"
#include "world/block.hpp"
#include "content/entity_manager.hpp"
#include "world/world.hpp"

#pragma once

class game {
    private:
        float delta_time;
        int fps = 0;
        int frame = 0;
        bool debug;

        screen_manager screen_mgr;

        world w;
    
    public:
        game(bool _debug = false) : screen_mgr("TripTrip", 1280, 720){
            debug = _debug;
        }
        
        int main_loop() {
            bool running = true;

            

            sf::Clock clock;
            float second_timer = 0;

            int mode;
            std::cout << "Mode (0 - host, 1 - client): ";
            std::cin >> mode;
            unsigned short port = 22355;
            
            sf::IpAddress address;
            sf::UdpSocket socket;

            socket.setBlocking(false);

            w.summonPlayer();
            w.summonPlayer();

            unsigned short responsePort;
            sf::Packet response;
            sf::IpAddress responseIP;

            if (mode == 0) {
                if (socket.bind(port) != sf::Socket::Done) {
                    std::cout << "Bind FAIL" << std::endl;
                    return -1;
                }
            } else if (mode == 1) {
                em::player_name = "player0";
                kb::new_bind(sf::Keyboard::A, "move_left");
                kb::new_bind(sf::Keyboard::D, "move_right");
                kb::new_bind(sf::Keyboard::Space, "jump");
                //std::string ip;
                //std::cin >> ip;
                //recipient = ip; 
                srvr::event e(em::player_name, "CONNECTED");
                srvr::eventPool.dump(&e);
                sf::Packet p = srvr::prepareEventPacket(&srvr::eventPool);
                address = "192.168.1.88";
                socket.send(p, address, port);

                p >> srvr::eventPool;
                std::cout << srvr::eventPool.pool.size() << "\n";
            }


            clock.restart();
            while (running) {
                delta_time = clock.restart().asSeconds();
                second_timer += delta_time;
                if (second_timer >= 1) {
                    second_timer = 0;
                    fps = frame;
                    wrld::fps = fps;
                    std::cout << fps << " fps" << std::endl;
                    frame = 0;
                }
                frame++;

                bool draw_colliders = true;

                //if (!srvr::eventPool.isEmpty()) {
                //    for (srvr::event i;; srvr::eventPool.iter(&i)) {
                //        std::cout << i.player << " " << i.type << std::endl;
                //    }
                //}


                w.resetCamera();

                screen_mgr.clear();

                screen_mgr.draw_blocks(w.block_mgr.tilemap); //needs optimization obviously
                screen_mgr.drawEntities(&w.alive_entity_mgr.entities, draw_colliders);

                {
                    em::updateGlobalEvents();
                    
                    if (mode == 0) {
                        if (socket.receive(response, responseIP, responsePort) == sf::Socket::Done) {
                            srvr::event_pool ep;
                            response >> ep;
                            srvr::event i;
                            while (ep.iter(&i)) {
                                //std::cout << "TEST2 " << i.type << " " << i.player << "\n";
                                if (i.type == "CONNECTED") print(i.player + " has connected.");
                                else srvr::eventPool.dump(&i);
                            }

                            sf::Packet p;

                            socket.send(p, responseIP, responsePort);
                        }

                    } else if (mode == 1) {
                        if (!srvr::eventPool.isEmpty()) {
                            sf::Packet p = srvr::prepareEventPacket(&srvr::eventPool);
                            std::cout << sizeof(p);
                            socket.send(p, address, port);
                        }

                        unsigned short responsePort;
                        sf::Packet response;
                        sf::IpAddress responseIP;
                        
                        if (socket.receive(response, responseIP, responsePort) == sf::Socket::Done) {
                            srvr::event_pool ep;
                            response >> ep;

                            srvr::event i;
                            while (ep.iter(&i)) {
                                srvr::eventPool.dump(&i);
                            }
                        }

                    } else return -1;

                    w.update(delta_time, fps);
                }

                running = screen_mgr.update();
            }
            return 0;
        }
};