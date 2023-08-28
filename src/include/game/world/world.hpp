#pragma once

#include "game/math/custom_round.hpp"
#include "game/content/entity_manager.hpp"
#include "game/content/block_manager.hpp"

class world {
    public:
        entity_manager alive_entity_mgr;
        block_manager block_mgr;

    public:
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

        std::vector<sf::IntRect> surroundingBlockColliders(int x, int y, int radius = 3) {
            return surroundingBlocksColiidersTile(int(customRound(x) / wrld::BLOCK_SIZE), int(customRound(y) / wrld::BLOCK_SIZE), radius);
        }

        std::vector<sf::IntRect> surroundingBlocksColiidersTile(int x, int y, int radius = 3) {
            std::vector<sf::IntRect> colliders;
            for (int i = x - radius; i < x + radius; i++) {
                if (!isInBoundsTile(i, 0)) continue;
                for (int j = y - radius; j < y + radius; j++) {
                    if (!isInBoundsTile(0, j)) continue;
                    if (block_mgr.is_solid(i, j)) {
                        sf::IntRect c = block_mgr.tilemap[i][j]->collider;
                        c.left += i * wrld::BLOCK_SIZE;
                        c.top += j * wrld::BLOCK_SIZE;
                        colliders.push_back(c);
                    }
                }
            }
            return colliders;
        }

        void checkEntityColliders(entity *e) {
            std::vector<sf::IntRect> colliders = surroundingBlockColliders(e->x, e->y);
            for (int i = 0; i < int(e->colliders.size()); i++) {
                sf::IntRect c = e->colliders.at(i).first;
                c.left += e->x;
                c.top += e->y;
                e->colliders.at(i).second = false;
                for (sf::IntRect j : colliders) {
                    if (c.intersects(j)) {
                        e->colliders.at(i).second = true;
                        break;
                    }
                }
            }
        }

        world() : block_mgr(true) {
            //summonPlayer();
        }

        void summonPlayer() {
            em::player_name = alive_entity_mgr.available_name("player");
            alive_entity_mgr.add_entity(wrld::entity_mgr.get_ptr("player"), em::player_name);
            entity *p = alive_entity_mgr.get_ptr(em::player_name);
            p->name = em::player_name;
        }

        void update(float delta_time, int fps = -1) {
            if (fps == -1) fps = 1/delta_time;
            
            entity *e;
            while (alive_entity_mgr.iter(&e)) {
                if (fps < 60) {
                    checkEntityColliders(e);
                    e->update(delta_time / 3);
                    checkEntityColliders(e);
                    e->update(delta_time / 3);
                    checkEntityColliders(e);
                    e->update(delta_time / 3);
                }
                else if (fps < 100) {
                    checkEntityColliders(e);
                    e->update(delta_time / 2);
                    checkEntityColliders(e);
                    e->update(delta_time / 2);
                }
                else {
                    checkEntityColliders(e);
                    e->update(delta_time);
                }
                e->after();
            }

            block_mgr.update(delta_time);
        }

        void actOnEvents() {
            entity *e;
            while (alive_entity_mgr.iter(&e)) {
                e->actOnEvents();
            }
        }

        void resetCamera() {
            wrld::camera_center = {float(alive_entity_mgr.get_ptr(em::player_name)->x), 
                                   float(alive_entity_mgr.get_ptr(em::player_name)->y)};
            wrld::camera_x = wrld::camera_center.x;
            wrld::camera_y = wrld::camera_center.y;
        }
};