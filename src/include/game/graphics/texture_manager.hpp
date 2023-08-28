#include "SFML/Graphics.hpp"

class texture_manager {
    public:
        texture_manager() {

        };
        
        sf::Sprite get_sprite(std::string texture_name, bool flipped) {
            return get_sprite(texture_name, -1, -1, -1, flipped);
        }
    
        sf::Sprite get_sprite(std::string texture_name, int frame = -1, int width = -1, int height = -1, bool flipped = false, int variant = 0) {
            sf::Sprite s;
            if (frame != -1) {
                s.setTextureRect(sf::IntRect({frame * width, width * variant}, {width, height}));
            }
            if (is_loaded(texture_name)) {
                s.setTexture(textures[texture_name]);
            }
            else {
                load(texture_name);
                s.setTexture(textures[texture_name]);
            }
            sf::IntRect r = s.getTextureRect();
            if (flipped) s.setTextureRect(sf::IntRect({r.left + r.width, r.top}, {-r.width, r.height}));
            return s;
        }

    private:
        bool is_loaded(std::string texture_name) {
            return (textures.find(texture_name) != textures.end());
        }

        bool load(std::string texture_name) {
            sf::Texture t;
            bool success = t.loadFromFile(textures_path + "/" + texture_name + ".png");
            if (!success) {
                success = t.loadFromFile(textures_path + "/" + missing_texture_name + ".png");
            }
            textures.insert(std::pair<std::string, sf::Texture>(texture_name, t));
            return success;
        }

        std::string missing_texture_name = "missing";
        std::string textures_path = "assets";
        std::map<std::string, sf::Texture> textures;
};

texture_manager texture_mgr;