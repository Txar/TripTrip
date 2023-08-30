#pragma once
#include "game/world/animated_block.hpp"
#include "game/world/background_block.hpp"
#include <fstream>

class block_manager {
    public:
        std::map<std::string, block> blocks;
        block ***tilemap;
        block none;

        const std::string level_directory = "assets/levels/";
        const std::string level_extension = ".ttl";

        sf::Vector2i player_spawn = {512, 512};

        std::string level_filename(std::string level_name) {
            return level_directory + level_name + level_extension;
        }

        bool add_block(block *b) {
            if (exists(b->name)) return false;
            else blocks.insert({b->name, *b});
            return true;
        }

        bool exists(std::string name) {
            return (blocks.find(name) != blocks.end());
        }

        block get(std::string name) {
            if (!exists(name)) return none;
            else return (blocks[name]);
        }

        block *get_ptr(std::string name) {
            if (!exists(name)) return &none;
            else return &(blocks[name]);
        }

        block_manager(bool load = false) {

            if (load) {
                block _none("none");

                block tile("tile");

                block _block("block");

                background_block spawn("spawn");
                spawn.visible = false;

                block spike("spike");
                spike.kills = true;

                animated_block fire("fire", 14, 6.0);
                fire.kills = true;

                background_block trash_can("trash_can");
                background_block wall("wall");
                background_block graffiti1("love_is_love");
                background_block graffiti2("pacifism");
                background_block graffiti3("eat_the_rich");
                background_block graffiti4("cat");

                add_block(&graffiti1);
                add_block(&graffiti2);
                add_block(&graffiti3);
                add_block(&graffiti4);

                add_block(&spawn);
                add_block(&_none);
                add_block(&tile);
                add_block(&wall);
                add_block(&spike);
                add_block(&trash_can);
                add_block(&_block);
                add_block(&fire);

                load_level("0");
            }
        }

        bool is_solid(int x, int y) {
            return tilemap[x][y]->solid;
        }

        void update(float delta_time) {
            for (auto i : blocks) {
                blocks[i.first].update(delta_time);
            }
        }

        int *decompress_blocks(std::string b) {
            int *result = new int[2];
            std::string num = "";
            int count = 0;
            for (char c : b) {
                if (c == 'x') {
                    count++;
                    result[0] = std::stoi(num);
                    num = "";
                } else {
                    num += c;
                }
            }
            result[1] = std::stoi(num);
            if (count == 0) {
                result[1] = std::stoi(num);
                result[0] = 1;
            }

            return result;
        }

        bool load_level(std::string level_name) {
            tilemap = new block **[wrld::WORLD_WIDTH];

            std::ifstream f;
            f.open(level_filename(level_name));

            std::string buffer;
            std::string prev = "";
            std::string status = "none";

            std::map<std::string, std::string> palette;
            std::map<std::string, std::string> properties;
            bool pair = false;

            std::vector<int> layout;

            while (f >> buffer) {
                if (buffer == "start") {
                    status = prev;
                } else if (buffer == "end") {
                    status = "none";
                } else if (buffer == "palette" || buffer == "properties" || buffer == "layout") {

                } else if (status != "none" && pair) {
                    if (status == "palette") {
                        palette.insert({prev, buffer});
                    } else if (status == "properties") {
                        properties.insert({prev, buffer});
                    }
                } else if (status != "none") {
                    if (status == "layout") {
                        int *b = decompress_blocks(buffer);
                        for (int i = 0; i < b[0]; i++) {
                            layout.push_back(b[1]);
                        }
                        pair = true;
                    }
                }
                pair = !pair;
                prev = buffer;
            }

            wrld::WORLD_WIDTH = std::stoi(properties.at("width"));
            wrld::WORLD_HEIGHT = std::stoi(properties.at("height"));

            int count = 0;
            for (int i = 0; i < wrld::WORLD_WIDTH; i++) {

                tilemap[i] = new block *[wrld::WORLD_HEIGHT];
                for (int j = 0; j < wrld::WORLD_HEIGHT; j++) {
                    block *b = get_ptr(palette.at(std::to_string(layout.at((j * wrld::WORLD_WIDTH) + i))));

                    if (b->name == "spawn") player_spawn = {i * wrld::BLOCK_SIZE, j * wrld::BLOCK_SIZE};

                    tilemap[i][j] = b;
                    count++;
                    //std::cout << b->name[0] << " ";
                }
            }

            return true;
        }
};

namespace wrld {
    //block_manager block_mgr(true);
}