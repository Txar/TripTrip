#pragma once

#include <iostream>
#include <cmath>
#include <algorithm>
#include "graphics/screen_manager.hpp"
#include "world/player_entity.hpp"
#include "world/block.hpp"
#include "content/entity_manager.hpp"
#include "world/world.hpp"
#include "game/graphics/ui.hpp"

class game {
    private:
        float delta_time;
        int fps = 0;
        int frame = 0;
        bool debug;

        screen_manager screen_mgr;

        world w;
        
        ui ui_mgr;
    
    public:
        game(bool _debug = false) : screen_mgr("TripTrip", 1280, 720), ui_mgr(true) {
            wrld::ui_mgr = &ui_mgr;
            debug = _debug;
            w.block_mgr.load_level("tutorial");
        }
        
        int main_loop() {
            bool running = true;

            sf::Clock clock;
            float second_timer = 0;

            w.summonPlayer();
            //entity *player = w.alive_entity_mgr.get_ptr("player0");

            kb::new_bind(sf::Keyboard::A, "move_left");
            kb::new_bind(sf::Keyboard::D, "move_right");
            kb::new_bind(sf::Keyboard::Space, "jump");
            kb::new_bind(sf::Keyboard::Left, "move_left");
            kb::new_bind(sf::Keyboard::Right, "move_right");
            kb::new_bind(sf::Keyboard::W, "jump");
            kb::new_bind(sf::Keyboard::Up, "jump");

            clock.restart();
            while (running) {
                delta_time = clock.restart().asSeconds();
                if (delta_time > 1.0/20.0) {
                    delta_time = 1.0/20.0;
                }
                second_timer += delta_time;
                if (second_timer >= 1) {
                    second_timer = 0;
                    fps = frame;
                    wrld::fps = fps;
                    frame = 0;
                }
                frame++;

                bool draw_colliders = false;

                //if (!srvr::eventPool.isEmpty()) {
                //    for (srvr::event i;; srvr::eventPool.iter(&i)) {
                //        std::cout << i.player << " " << i.type << std::endl;
                //    }
                //}



                screen_mgr.clear();
                screen_mgr.drawBackground(ui_mgr.drawBackgrounds);
                screen_mgr.drawBlocks(w.block_mgr.tilemap); //needs optimization obviously
                screen_mgr.drawSeeds(w.block_mgr.seeds, delta_time);
                screen_mgr.drawEntities(&w.alive_entity_mgr.entities, draw_colliders);
                screen_mgr.drawUI(ui_mgr);
                w.resetCamera(screen_mgr.screen_width, screen_mgr.screen_height);
                screen_mgr.moveCamera();

                wrld::sound_mgr.updateVolume();
                wrld::sound_mgr.play_sounds();
                em::updateGlobalEvents();

                if (!wrld::paused) {
                    w.actOnEvents();
                    w.update(delta_time, fps);
                } else {
                    ui_mgr.scene = "paused";
                }
                ui_mgr.update();

                running = screen_mgr.update();
            }
            return 0;
        }
};