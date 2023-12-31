#ifndef SFML_TEXTURE_H
#define SFML_TEXTURE_H

#include <SFML/Graphics.hpp>

#include "../drawable.h"
#include "../../standard/plug_texture.h"
#include "../intrect/intrect.h"

class Sprite : public Drawable
{
    sf::Texture sprite_texture;
    const plug::Texture *plug_texture = nullptr;

public:
    Sprite ();
    Sprite (const plug::Texture &texture);
    Sprite (const char *path) {drawable = new sf::Sprite; assert (drawable); loadFromFile (path);};
    ~Sprite ();

    Sprite *clone () const override {return nullptr;};
    
    void setTexture (const plug::Texture &texture);
    void setTextureRect (IntRect &rect);
    void setPosition (const plug::Vec2d pos) override;
    void setPosition (double x, double y) override;
    plug::Vec2d getPosition () const override;
    const plug::Texture *getTexture () const;
    void loadFromFile (const char *pathname);
};




#endif /* SFML_TEXTURE_H */