#pragma once
#include "game/world/animated_block.hpp"
#include "game/world/background_block.hpp"
#include <fstream>
#include <vector>

class block_manager {
    public:
        std::map<std::string, block> blocks;
        block ***tilemap;
        block none;

        sf::Clock clock;

        std::string level_to_load = "0";

        std::vector<sf::Vector2i> seeds;

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

                block tile("tile", true);

                block _block("block", true);

                background_block spawn("spawn");
                spawn.visible = false;

                background_block finish("finish");
                finish.visible = false;
                finish.is_finish = true;
                add_block(&finish);

                background_block street_light("street_light");
                add_block(&street_light);

                background_block pillar("pillar");
                add_block(&pillar);

                background_block tire_0("tire_0");
                add_block(&tire_0);

                background_block heart("heart");
                add_block(&heart);

                block asphalt("asphalt");
                add_block(&asphalt);

                block spike("spike");
                spike.kills = true;
                spike.collider = sf::IntRect(16, 24, 32, 40);

                block spike_wall("spike_wall");
                spike_wall.kills = true;
                spike_wall.is_background = true;
                spike_wall.collider = sf::IntRect(16, 24, 32, 40);
                add_block(&spike_wall);

                animated_block fire("fire", 14, 6.0);
                fire.kills = true;

                animated_block moving_1_0("moving_1_0", 9, 6.0, true, 0);
                moving_1_0.anim.name = "moving_1";
                moving_1_0.animated_collider = true;
                moving_1_0.animatedColliders = {
                    sf::IntRect(0, 0, 16, 16),
                    sf::IntRect(0, 0, 14, 16),
                    sf::IntRect(0, 0, 12, 16),
                    sf::IntRect(0, 0, 10, 16),
                    sf::IntRect(0, 0, 8, 16),
                    sf::IntRect(0, 0, 6, 16),
                    sf::IntRect(0, 0, 4, 16),
                    sf::IntRect(0, 0, 2, 16),
                    sf::IntRect(0, 0, 0, 16)
                };
                add_block(&moving_1_0);

                animated_block moving_1_1("moving_1_1", 9, 6.0, true, 1);
                moving_1_1.anim.name = "moving_1";
                add_block(&moving_1_1);

                animated_block moving_1_2("moving_1_2", 9, 6.0, true, 2);
                moving_1_2.anim.name = "moving_1";
                moving_1_2.animated_collider = true;
                moving_1_2.animatedColliders = {
                    sf::IntRect(0, 0, 0, 16),
                    sf::IntRect(0, 0, 2, 16),
                    sf::IntRect(0, 0, 4, 16),
                    sf::IntRect(0, 0, 6, 16),
                    sf::IntRect(0, 0, 8, 16),
                    sf::IntRect(0, 0, 10, 16),
                    sf::IntRect(0, 0, 12, 16),
                    sf::IntRect(0, 0, 14, 16),
                    sf::IntRect(0, 0, 16, 16)
                };
                add_block(&moving_1_2);

                block bricks("bricks", true);
                add_block(&bricks);

                block brick_floor("brick_floor", true);
                add_block(&brick_floor);

                background_block support_left("support_left");
                add_block(&support_left);
                background_block support_right("support_right");
                add_block(&support_right);

                block left_half("left_half");
                left_half.collider = sf::IntRect(0, 0, (int) wrld::BLOCK_SIZE / 2.0, wrld::BLOCK_SIZE);
                add_block(&left_half);

                block right_half("right_half");
                right_half.collider = sf::IntRect((int) wrld::BLOCK_SIZE / 2, 0, (int) wrld::BLOCK_SIZE / 2, wrld::BLOCK_SIZE);
                add_block(&right_half);

                block easter_egg("none");
                add_block(&easter_egg);

                background_block trash_can("trash_can");
                background_block wall("wall");
                background_block graffiti1("love_is_love");
                background_block graffiti2("pacifism");
                background_block graffiti3("eat_the_rich");
                background_block graffiti4("cat");
                background_block graffiti5("fist");
                background_block graffiti6("triptrip");

                add_block(&graffiti1);
                add_block(&graffiti2);
                add_block(&graffiti3);
                add_block(&graffiti4);
                add_block(&graffiti5);
                add_block(&graffiti6);

                add_block(&spawn);
                add_block(&_none);
                add_block(&tile);
                add_block(&wall);
                add_block(&spike);
                add_block(&trash_can);
                add_block(&_block);
                add_block(&fire);
            }
        }

        bool is_solid(int x, int y) {
            return tilemap[x][y]->solid;
        }

        void update(float delta_time) {
            for (auto i : blocks) {
                blocks.at(i.first).update(delta_time);
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
            if (tilemap) {
                delete [] tilemap;
            }

            wrld::ui_mgr->scene = "message";

            seeds.clear();

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
            if (properties.at("music") != "none") {
                wrld::sound_mgr.loadMusic(properties.at("music"));
            }

            wrld::draw_console = false;
            if (properties.find("last") != properties.end() && properties.at("last") == "true") {
                float time = clock.getElapsedTime().asSeconds();
                print("This is the end!\nThanks for playing! <3\nDeaths: "
                + std::to_string(wrld::death_counter) + "\nSeeds collected: " 
                + std::to_string(wrld::collected_seeds) + "/" + std::to_string(wrld::all_seeds) 
                + "\nJumps: " + std::to_string(wrld::jump_counter) 
                + "\nTotal time: " + std::to_string((int) floor(time / 60.0)) + " minutes " + std::to_string(int(floor(time)) % 60) + " seconds (" + std::to_string(time) + " seconds)");
                wrld::draw_console = true;

                if (wrld::all_seeds == wrld::collected_seeds) {
                    texture_mgr.anastasia_unlocked = true;
                    print("\nYou collected all the seeds! Congrats.\nAs a prize, you get a new special button at the bottom of the screen.");
                }
            }

            tilemap = new block **[wrld::WORLD_WIDTH];

            level_to_load = properties.at("next");

            int count = 0;
            for (int i = 0; i < wrld::WORLD_WIDTH; i++) {
                tilemap[i] = new block *[wrld::WORLD_HEIGHT];
                for (int j = 0; j < wrld::WORLD_HEIGHT; j++) {
                    std::string b_name = palette.at(std::to_string(layout.at((j * wrld::WORLD_WIDTH) + i)));
                    block *b = get_ptr(b_name);

                    if (b_name == "spawn") player_spawn = {i * wrld::BLOCK_SIZE, j * wrld::BLOCK_SIZE};

                    if (b_name == "seed") {
                        seeds.push_back({i, j});
                        wrld::all_seeds++;
                    }
                    tilemap[i][j] = b;
                    count++;
                }
            }

            if (wrld::level > 4) wrld::level = 1;

            return true;
        }
};

namespace wrld {
    //block_manager block_mgr(true);
}