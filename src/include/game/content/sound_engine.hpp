#pragma once

#include "SFML/Audio.hpp"
#include <map>
#include <iostream>

namespace wrld {
    bool music_on = true;
    bool sound_on = true;
}

class sound_engine {
    public:
        std::map<std::string, sf::SoundBuffer> sound_buffers;
        std::map<std::string, sf::Sound> sounds;

        float music_volume = 1.0;
        float sound_volume = 1.0;

        std::vector<std::string> queue;

        std::string sounds_path = "assets/";

        sf::Music music;

        std::vector<std::string> sounds_to_play;

        sound_engine() {}

        void updateVolume() {
            music.setVolume(100.0 * sound_volume * wrld::music_on);
            play_music();
        }

        void loadMusic(std::string name, std::string file_format = "ogg") {
            queue.push_back(name + "." + file_format);
        }

        void play_music() {
            if ((int)queue.size() > 0 && music.getStatus() == sf::Music::Stopped) {
                if (music.openFromFile(sounds_path + queue.at(0))) music.play();
                else std::cout << "oh NO\n";
                queue.erase(queue.begin());
            }
        }

        void play_sounds() {
            for (std::string i : sounds_to_play) {
                sounds.at(i).setVolume(100.0 * sound_volume * wrld::sound_on);
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