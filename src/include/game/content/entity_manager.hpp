#include "game/world/player_entity.hpp"

#pragma once

class entity_manager {
    public:
        std::map<std::string, entity> base_entities;
        std::map<std::string, box_entity> box_entities;
        std::map<std::string, player_entity> player_entities;
        std::vector<entity*> entities;
        entity none = entity("none", 0.0, 0.0, 0.0);
        int iterator = 0;

        bool iter(entity **e) {
            if (iterator == int(entities.size())) {
                iterator = 0;
                return false;
            }
            else {
                (*e) = entities.at(iterator);
                iterator++;
                return true;
            }
        }

        void update(float delta_time) {
            for (auto i : base_entities) base_entities.at(i.first).update(delta_time);
            for (auto i : box_entities) box_entities.at(i.first).update(delta_time);
            for (auto i : player_entities) player_entities.at(i.first).update(delta_time);
        }

        bool add_entity(entity *e, std::string name = "") {
            if (name == "") name = (*e).name;
            if (exists(name)) return false;
            if ((*e).type == "baseEntity") {
                base_entities.insert({name, *e});
                entities.push_back(&base_entities.at(name));
            }
            else if ((*e).type == "boxEntity") {
                box_entities.insert({name, box_entity(*e)});
                entities.push_back(&box_entities.at(name));
            }
            else if ((*e).type == "playerEntity") {
                player_entities.insert({name, player_entity(*e)});
                entities.push_back(&player_entities.at(name));
            }
            return true;
        }

        std::string available_name(std::string preferredName) {
            int i = 0;
            while (exists(preferredName + std::to_string(i))) {
                i++;
            }
            return preferredName + std::to_string(i);
        }

        bool exists(std::string name) {
            bool e = (base_entities.find(name) != base_entities.end())
             || (box_entities.find(name) != box_entities.end())
             || (player_entities.find(name) != player_entities.end());
            return e;
        }

        std::string type(std::string name) {
            if (base_entities.find(name) != base_entities.end()) return "baseEntity";
            if (box_entities.find(name) != box_entities.end()) return "boxEntity";
            if (player_entities.find(name) != player_entities.end()) return "playerEntity";
            else return "none";
        }

        entity *get_ptr(std::string name) {
            std::string t = type(name);
            if (t == "baseEntity") return &base_entities.at(name);
            if (t == "boxEntity") return &box_entities.at(name);
            if (t == "playerEntity") return &player_entities.at(name);
            else return &none;
        }

        entity_manager(bool load = false) {
            if (load) {
                player_entity p = player_entity("player", 128, 128);
                add_entity(&p);
                box_entity e = box_entity("box", 128, 128);
                e.animators.push_back(animator("Anastasia-run", 128, 128, 6, 12.0));
                e.applyGravity = true;
                e.doApplyMovement = true;
                add_entity(&e);
            }
        }
};

namespace wrld {
    entity_manager entity_mgr(true);
}