#pragma once

#include "SFML/Audio.hpp"
#include <map>
#include <iostream>

class sound_engine {
    public:
        std::map<std::string, sf::SoundBuffer> sound_buffers;
        std::map<std::string, sf::Sound> sounds;
        std::string sounds_path = "assets/";

        std::vector<std::string> sounds_to_play;

        sound_engine() {

        }

        void play_sounds() {
            for (std::string i : sounds_to_play) {
                sounds.at(i).play();
            }
            sounds_to_play.clear();
        }

        void play(std::string name) {
            sounds_to_play.push_back(name);
        }

        bool load(std::string name, std::string file_format = "wav") {
            sf::SoundBuffer s;
            bool success = s.loadFromFile(sounds_path + name + "." + file_format);
            sound_buffers.insert({name, s});
            sounds.insert({name, sf::Sound(sound_buffers.at(name))});
            return success;
        }
};

namespace wrld {
    sound_engine sound_mgr;
}