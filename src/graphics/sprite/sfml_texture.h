#ifndef SFML_TEXTURE_H
#define SFML_TEXTURE_H

#include <SFML/Graphics.hpp>

#include "../drawable.h"


// struct SFMLTexture
// {
//     sf::Texture texture;
//     SFMLTexture () = default;
//     SFMLTexture (const SFMLTexture &other) : texture (other.texture) {};
    
//     ~SFMLTexture () = default;

//     bool loadFromFile (const char *pathname) {return texture.loadFromFile (pathname);};
// };
// struct SFMLTexture : public plug::Texture
// {
//     sf::Texture texture;

//     SFMLTexture () = default; //
    
//     SFMLTexture (const SFMLTexture &other) : texture (other.texture) {}; // 
//     SFMLTexture(size_t width, size_t height, const Color* data) :
//             plug::Texture (width, height, data) {};

//     SFMLTexture(size_t width, size_t height) :
//             plug::Texture (width, height) {};

//     ~SFMLTexture () = default;

//     bool loadFromFile (const char *pathname) {return texture.loadFromFile (pathname);};
// };

class Sprite : public Drawable
{
    sf::Texture sprite_texture;
    const plug::Texture *plug_texture;

public:
    Sprite ();
    Sprite (const plug::Texture &texture);
    ~Sprite ();

    void setTexture (const plug::Texture &texture);
    void setPosition (double x, double y);
    void setPosition (const plug::Vec2d &pos);
    const plug::Texture *getTexture () const;
    void loadFromFile (const char *pathname);
};




#endif /* SFML_TEXTURE_H */