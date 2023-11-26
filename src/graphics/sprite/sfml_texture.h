#ifndef SFML_TEXTURE_H
#define SFML_TEXTURE_H

#include <SFML/Graphics.hpp>

#include "../drawable.h"
#include "../../standard/plug_texture.h"
#include "../intrect/intrect.h"

class Sprite : public Drawable
{
    sf::Texture sprite_texture;
    const plug::Texture *plug_texture;

public:
    Sprite ();
    Sprite (const plug::Texture &texture);
    ~Sprite ();

    void setTexture (const plug::Texture &texture);
    void setTextureRect (IntRect &rect);
    void setPosition (double x, double y);
    void setPosition (const plug::Vec2d &pos);
    const plug::Texture *getTexture () const;
    void loadFromFile (const char *pathname);
};




#endif /* SFML_TEXTURE_H */