#include "sfml_texture.h"
#include "../convert_sfml_functions.h"

Sprite::Sprite ()
{
    drawable = new sf::Sprite;
    assert (drawable);
}

Sprite::Sprite (const plug::Texture &texture)
{
    copyToSFMLTexture (sprite_texture, texture);
    
    drawable = new sf::Sprite (sprite_texture);
    assert (drawable);
  
    plug_texture = &texture;
}

Sprite::~Sprite ()
{
    if (drawable)
    {
        delete drawable;
        drawable = nullptr;
        plug_texture = nullptr;
    }
}

void Sprite::setTexture (const plug::Texture &texture)
{
    copyToSFMLTexture (sprite_texture, texture);
    ((sf::Sprite *)drawable)->setTexture (sprite_texture);
    
    plug_texture = &texture;
}

void Sprite::setPosition (double x, double y)
{
    ((sf::Sprite *)drawable)->setPosition (x, y);
}

void Sprite::setPosition (const plug::Vec2d &pos)
{
    ((sf::Sprite *)drawable)->setPosition (getSFMLVector2f (pos));
}

const plug::Texture *Sprite::getTexture () const
{
    return plug_texture;
}

void Sprite::loadFromFile (const char *pathname)
{
    sprite_texture.loadFromFile (pathname);
    ((sf::Sprite *)drawable)->setTexture (sprite_texture);
}