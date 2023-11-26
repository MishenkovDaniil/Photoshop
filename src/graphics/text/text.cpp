#include "text.h"
#include "../convert_sfml_functions.h"

Text::Text ()
{
    drawable = new sf::Text;
    assert (drawable);
}

Text::~Text ()
{
    if (drawable)
    {
        delete drawable;
        drawable = nullptr;
    }
    font_ = nullptr;
}

void Text::setString (const char *string)
{
    sf::String sfml_string (string);
    ((sf::Text *)drawable)->setString (sfml_string);
}

void Text::setFont (const Font &font)
{
    font_ = &font;
    ((sf::Text *)drawable)->setFont (font.font);
}

void Text::setFillColor (const plug::Color &color)
{
    ((sf::Text *)drawable)->setFillColor (getSFMLColor (color));
}

void Text::setCharacterSize (size_t character_size)
{
    ((sf::Text *)drawable)->setCharacterSize (character_size);
}


plug::Vec2d Text::findCharacterPos(size_t character)
{
    sf::Vector2f pos = ((sf::Text *)drawable)->findCharacterPos (character);
    return plug::Vec2d (pos.x, pos.y);
}

void Text::setPosition (double x, double y)
{
    ((sf::Text *)drawable)->setPosition (x, y);
}

void Text::setPosition (const plug::Vec2d &pos)
{
    ((sf::Text *)drawable)->setPosition (getSFMLVector2f (pos));
}


const char *Text::getString () const
{
    const sf::String &string = ((sf::Text *)drawable)->getString ();
    return (const char *)string.getData ();
}

const Font Text::getFont () const
{
    assert (font_);
    return *font_;
}

plug::Color Text::getFillColor () const
{
    sf::Color fill_color = ((sf::Text *)drawable)->getFillColor ();
    return plug::Color (fill_color.r, fill_color.g, fill_color.b, fill_color.a);
}

size_t Text::getCharacterSize () const
{
    return ((sf::Text *)drawable)->getCharacterSize ();
}

plug::Vec2d Text::getPosition () const
{
    sf::Vector2f pos = ((sf::Text *)drawable)->getPosition ();
    return plug::Vec2d (pos.x, pos.y);
}
