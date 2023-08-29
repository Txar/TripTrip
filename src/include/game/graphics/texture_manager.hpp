#include "SFML/Graphics.hpp"

class texture_manager {
    public:
        float onload_scaling = 1.0;

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
            bool success = true;
            if (texture_name == "none") t.loadFromFile(textures_path + "/none.png");
            else {
                success = t.loadFromFile(textures_path + "/" + texture_name + ".png");
                if (!success) {
                    success = t.loadFromFile(textures_path + "/" + missing_texture_name + ".png");
                }
            }
            if (onload_scaling == 1.0) {
                textures.insert(std::pair<std::string, sf::Texture>(texture_name, t));
            } else {
                sf::RenderTexture t2;
                t2.create(t.getSize().x * onload_scaling, t.getSize().y * onload_scaling);
                sf::Sprite s(t);
                s.setScale(onload_scaling, onload_scaling);
                t2.draw(s);
                t2.display();
                textures.insert(std::pair<std::string, sf::Texture>(texture_name, t2.getTexture()));
            }
            return success;
        }

        std::string missing_texture_name = "missing";
        std::string textures_path = "assets";
        std::map<std::string, sf::Texture> textures;
};

texture_manager texture_mgr;