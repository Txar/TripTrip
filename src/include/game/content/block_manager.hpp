#pragma once
#include "game/world/animated_block.hpp"
#include <fstream>

class block_manager {
    public:
        std::map<std::string, block> blocks;
        block ***tilemap;
        block none;

        const std::string level_directory = "assets/levels";
        const std::string level_extension = ".tll";

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
                block wall("wall");
                add_block(&_none);
                add_block(&tile);
                add_block(&wall);

                for (int i = 0; i < wrld::WORLD_WIDTH; i++) {
                    tilemap[i] = new block *[wrld::WORLD_HEIGHT];
                    for (int j = 0; j < wrld::WORLD_HEIGHT; j++) {
                        tilemap[i][j] = &none;
                        if (i == 0 || j == 5 || (j == 2 && i > 7)) {
                            tilemap[i][j] = get_ptr("test_anim");
                        }
                    }
                }
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
                    result[count] = std::stoi(num);
                    num = "";
                } else {
                    num += c;
                }
            }
            if (count == 0) {
                result[1] = result[0];
            }
            return result;
        }

        bool load_level(std::string level_name) {
            tilemap = new block **[wrld::WORLD_WIDTH];

            std::ifstream f;
            f.open(level_filename(level_name));

            int width, height;
            std::string buffer;
            std::string prev;
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
                } else if (status != "none" && pair) {
                    if (status == "palette") {
                        palette.insert(prev, buffer);
                    } else if (status == "properties") {
                        properties.insert(prev, buffer);
                    }
                } else if (status != "none") {
                    if (status == "layout") {
                        
                    }
                }

                pair = !pair;
            }

            for (int i = 0; i < wrld::WORLD_WIDTH; i++) {
                tilemap[i] = new block *[wrld::WORLD_HEIGHT];
                for (int j = 0; j < wrld::WORLD_HEIGHT; j++) {

                }
            }

            return true;
        }
};

namespace wrld {
    //block_manager block_mgr(true);
}