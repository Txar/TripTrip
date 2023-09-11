#pragma once

#include "game/io/events.hpp"
#include "game/content/sound_engine.hpp"
#include "SFML/Graphics.hpp"
#include "game/graphics/button.hpp"
#include "game/world/entity.hpp"
#include "game/graphics/text_button.hpp"

class ui {
    public:
        std::vector<std::string> actions;
        std::vector<button> buttons;

        std::string scene = "game";

        int iterator = 0;
        bool drawBackgrounds = true;

        ui(bool load = false) {
            if (load) {
                button sound_mute("sound_button", true, al::LEFT, al::BOTTOM, 0, 0);
                sound_mute.action = "sound_mute";
                buttons.push_back(sound_mute);

                button music_mute("music_button", true, al::LEFT, al::BOTTOM, 32, 0);
                music_mute.action = "music_mute";
                buttons.push_back(music_mute);

                button background_button("background_button", true, al::CENTER, al::MIDDLE, 0, 128);
                background_button.action = "background_switch";
                background_button.scene = "paused";
                buttons.push_back(background_button);

                button pause("pause_button", false, al::RIGHT, al::BOTTOM, 0, 0);
                pause.freeze = true;
                pause.action = "pause";
                pause.scene = "game";
                buttons.push_back(pause);

                button unpause("pause_button", false, al::CENTER, al::MIDDLE, 0, 0);
                unpause.freeze = true;
                unpause.anim.frame = 1;
                unpause.action = "pause";
                unpause.scene = "paused";
                buttons.push_back(unpause);

                text_button pause_title("pause_title", al::CENTER, al::MIDDLE, 0, - (2 * wrld::BLOCK_SIZE));
                pause_title.scene = "paused";
                buttons.push_back(pause_title);

                text_button message0("tutorial", al::CENTER, al::MIDDLE, 0, 0);
                message0.scene = "message";
                message0.level = 0;
                buttons.push_back(message0);

                text_button message("paper_0", al::CENTER, al::MIDDLE, 0, 0);
                message.scene = "message";
                message.level = 1;
                buttons.push_back(message);

                text_button message1("paper_1", al::CENTER, al::MIDDLE, 0, 0);
                message1.scene = "message";
                message1.level = 2;
                buttons.push_back(message1);

                text_button message2("paper_2", al::CENTER, al::MIDDLE, 0, 0);
                message2.scene = "message";
                message2.level = 3;
                buttons.push_back(message2);

                text_button message3("paper_3", al::CENTER, al::MIDDLE, 0, 0);
                message3.scene = "message";
                message3.level = 4;
                buttons.push_back(message3);

                text_button close("ok", al::CENTER, al::BOTTOM, 0, - ((int) wrld::BLOCK_SIZE / 2));
                close.scene = "message";
                close.action = "close_message";
                buttons.push_back(close);

                text_button tog("anastasia_toggle", al::LEFT, al::BOTTOM, 64, 0);
                tog.action = "toggle_anastasia";
                buttons.push_back(tog);
            }
        }

        void addAction(std::string action) {
            actions.push_back(action);
        }

        bool iter(button *b) {
            if (iterator == int(buttons.size())) {
                iterator = 0;
                return false;
            }
            else {
                (*b) = buttons.at(iterator);
                iterator++;
                return true;
            }
        }

        void update() {
            for (int i = 0; i < (int) buttons.size(); i++) {
                if (scene != "none" && buttons.at(i).scene != "all" && buttons.at(i).scene != scene) continue;
                if (em::mouseHasBeenPressed && buttons.at(i).isPressed(em::mouse_x, em::mouse_y)) {
                    buttons.at(i).press();
                    actions.push_back(buttons.at(i).action);
                }
                buttons.at(i).update();
            }
            executeActions();
        }

        void executeActions() {
            for (std::string i : actions) {
                if (i == "sound_up") {
                    if (wrld::sound_mgr.sound_volume < 1.0) {
                        wrld::sound_mgr.sound_volume += 0.1;
                    }
                } else if (i == "sound_down") {
                    if (wrld::sound_mgr.sound_volume > 0.0) {
                        wrld::sound_mgr.sound_volume -= 0.1;
                    }
                } else if (i == "sound_mute") {
                    wrld::sound_on = !wrld::sound_on;
                } else if (i == "music_up") {
                    if (wrld::sound_mgr.music_volume < 1.0) {
                        wrld::sound_mgr.music_volume += 0.1;
                    }
                } else if (i == "music_down") {
                    if (wrld::sound_mgr.music_volume > 0.0) {
                        wrld::sound_mgr.music_volume -= 0.1;
                    }
                } else if (i == "music_mute") {
                    wrld::music_on = !wrld::music_on;
                } else if (i == "pause") {
                    if (scene == "game") scene = "paused";
                    else scene = "game";
                    wrld::paused = !wrld::paused;
                }
                else if (i == "background_switch") {
                    drawBackgrounds = !drawBackgrounds;
                }
                else if (i == "close_message") {
                    scene = "game";
                }
                else if (i == "toggle_anastasia") {
                    texture_mgr.anastasia_on = !texture_mgr.anastasia_on;
                }
            }
            actions.clear();
        }
};

namespace wrld {
    ui *ui_mgr;
}