#pragma once

#include "game/math/custom_round.hpp"
#include "game/content/entity_manager.hpp"
#include "game/content/block_manager.hpp"

class world {
    public:
        entity_manager alive_entity_mgr;
        block_manager block_mgr;

    public:
        bool load_world = false;

        bool isInBounds(int x, int y) {
            return isInBounds(int(customRound(x) / wrld::BLOCK_SIZE), int(customRound(y) / wrld::BLOCK_SIZE));
        }

        bool isInBoundsTile(int x, int y) {
            return (x < wrld::WORLD_WIDTH * wrld::BLOCK_SIZE && x >= 0
                && y < wrld::WORLD_HEIGHT * wrld::BLOCK_SIZE && y >= 0);
        }

        sf::IntRect blockCollider(int x, int y) {
            return blockCollider(int(customRound(x) / wrld::BLOCK_SIZE), int(customRound(y) / wrld::BLOCK_SIZE));
        }

        sf::IntRect blockColliderTile(int x, int y) {
            return sf::IntRect(x * wrld::BLOCK_SIZE, y * wrld::BLOCK_SIZE, 
            wrld::BLOCK_SIZE, wrld::BLOCK_SIZE);
        }

        std::vector<std::pair<sf::IntRect, block*>> surroundingBlockColliders(int x, int y, int radius = 3) {
            return surroundingBlocksColiidersTile(int(customRound(x) / wrld::BLOCK_SIZE), int(customRound(y) / wrld::BLOCK_SIZE), radius);
        }

        std::vector<std::pair<sf::IntRect, block*>> surroundingBlocksColiidersTile(int x, int y, int radius = 3) {
            std::vector<std::pair<sf::IntRect, block*>> colliders;
            for (int i = x - radius; i < x + radius; i++) {
                if (!isInBoundsTile(i, 0)) continue;
                for (int j = y - radius; j < y + radius; j++) {
                    if (!isInBoundsTile(0, j)) continue;
                    if (block_mgr.is_solid(i, j) || block_mgr.tilemap[i][j]->is_finish || block_mgr.tilemap[i][j]->kills) {
                        sf::IntRect c = block_mgr.tilemap[i][j]->collider;
                        c.left += i * wrld::BLOCK_SIZE;
                        c.top += j * wrld::BLOCK_SIZE;
                        colliders.push_back({c, block_mgr.tilemap[i][j]});
                    }
                }
            }
            return colliders;
        }

        void checkEntityColliders(entity *e) {
            std::vector<std::pair<sf::IntRect, block*>> colliders = surroundingBlockColliders(e->x, e->y);
            for (int i = 0; i < int(e->colliders.size()); i++) {
                sf::IntRect c = e->colliders.at(i).first;
                c.left += e->x;
                c.top += e->y;
                e->colliders.at(i).second = false;

                bool killed = false;

                for (int j = 0; j < (int) block_mgr.seeds.size(); j++) {
                    sf::IntRect rect = {block_mgr.seeds.at(j).x * wrld::BLOCK_SIZE, block_mgr.seeds.at(j).y * wrld::BLOCK_SIZE, wrld::BLOCK_SIZE, wrld::BLOCK_SIZE};
                    if (c.intersects(rect)) {
                        block_mgr.seeds.erase(block_mgr.seeds.begin() + j);
                        wrld::sound_mgr.play("seed");
                        wrld::collected_seeds++;
                        break;
                    }
                }

                for (auto j : colliders) {
                    if (!killed && j.second->kills) {
                        for (int i = 0; i < int(e->damageColliders.size()); i++) {
                            sf::IntRect c = e->damageColliders.at(i).first;
                            c.left += e->x;
                            c.top += e->y;
                            if (c.intersects(j.first)) {
                                e->damageColliders.at(i).second = true;
                                killed = true;
                            }
                        }

                    } else if (c.intersects(j.first)) {
                        e->colliders.at(i).second = true;
                        if (j.second->is_finish) {
                            load_world = true;
                        }
                        break;
                    }
                }
            }
        }

        world() : block_mgr(true) {
            entity *spawn = alive_entity_mgr.get_ptr("spawn");
            wrld::sound_mgr.load("seed");
            spawn->x = block_mgr.player_spawn.x;
            spawn->y = block_mgr.player_spawn.y;

            //summonPlayer();
        }

        void summonPlayer() {
            wrld::player_is_alive = true;
            em::player_name = alive_entity_mgr.available_name("player");
            alive_entity_mgr.add_entity(wrld::entity_mgr.get_ptr("player"), em::player_name);
            entity *p = alive_entity_mgr.get_ptr(em::player_name);
            p->name = em::player_name;
            p->x = block_mgr.player_spawn.x;
            p->y = block_mgr.player_spawn.y;
            p->alive = true;
        }

        void update(float delta_time, int fps = -1) {
            if (fps == -1) fps = 1/delta_time;
            
            entity *e;
            
            std::vector<int> entities_to_kill;

            block_mgr.update(delta_time);

            while (alive_entity_mgr.iter(&e)) {
                if (!e->alive) {
                    entities_to_kill.push_back(alive_entity_mgr.iterator - 1);
                    continue;
                }

                if (fps < 60) {
                    checkEntityColliders(e);
                    e->update(delta_time / 4.0);
                    checkEntityColliders(e);
                    e->update(delta_time / 4.0);
                    checkEntityColliders(e);
                    e->update(delta_time / 4.0);
                    checkEntityColliders(e);
                    e->update(delta_time / 4.0);
                }
                else if (fps < 100) {
                    checkEntityColliders(e);
                    e->update(delta_time / 3.0);
                    checkEntityColliders(e);
                    e->update(delta_time / 3.0);
                    checkEntityColliders(e);
                    e->update(delta_time / 3.0);
                }
                else {
                    checkEntityColliders(e);
                    e->update(delta_time);
                }
                e->after();
            }

            for (int i = 0; i < (int) entities_to_kill.size(); i++) {
                int index = entities_to_kill.at(i) - i;
                alive_entity_mgr.remove(index);
            }

            if (!wrld::player_is_alive) {
                summonPlayer();
            }

            if (load_world) {
                block_mgr.load_level(block_mgr.level_to_load);
                alive_entity_mgr = entity_manager();
                entity *spawn = alive_entity_mgr.get_ptr("spawn");
                spawn->x = block_mgr.player_spawn.x;
                spawn->y = block_mgr.player_spawn.y;
                wrld::player_is_alive = false;
                load_world = false;
                wrld::level++;
            }
        }   

        void actOnEvents() {
            entity *e;
            while (alive_entity_mgr.iter(&e)) {
                e->actOnEvents();
            }
        }

        void resetCamera(int screen_width, int screen_height) {
            entity *p = alive_entity_mgr.get_ptr(em::player_name);
            wrld::camera_center = {float(p->x + ((float)p->width / 2)),
                                   float(p->y + ((float)p->height / 2))};
            wrld::camera_x = wrld::camera_center.x;
            wrld::camera_y = wrld::camera_center.y;

            if (wrld::camera_x - screen_width/2 < 0) {
                wrld::camera_x = screen_width/2;
            } else if (wrld::camera_x + screen_width/2 > wrld::WORLD_WIDTH * wrld::BLOCK_SIZE) {
                wrld::camera_x = wrld::WORLD_WIDTH * wrld::BLOCK_SIZE - screen_width/2;
            }
            if (wrld::camera_y - screen_height/2 < 0) {
                wrld::camera_y = screen_height/2;
            } else if (wrld::camera_y + screen_height/2 > wrld::WORLD_HEIGHT * wrld::BLOCK_SIZE) {
                wrld::camera_y = wrld::WORLD_HEIGHT * wrld::BLOCK_SIZE - screen_height/2;
            }
        }
};