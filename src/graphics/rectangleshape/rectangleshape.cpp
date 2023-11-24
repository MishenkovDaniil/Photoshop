#include "rectangleshape.h"

RectangleShape::RectangleShape (const plug::Vec2d &size = plug::Vec2d (0, 0))
    : Shape (static_cast<size_t>(Shapes::Rectangle)),
      sfml_rect(getSFMLVector2f (size)) {}


void RectangleShape::setSize (const plug::Vec2d &size)
{
    sfml_rect.setSize (getSFMLVector2f (size));
}

void RectangleShape::setFillColor (const plug::Color color)
{
    sfml_rect.setFillColor (getSFMLColor (color));
}

void RectangleShape::setOutlineColor (const plug::Color color)
{
    sfml_rect.setOutlineColor (getSFMLColor (color));
}

void RectangleShape::setOutlineThickness (double thickness)
{
    sfml_rect.setOutlineThickness (thickness);
}

void RectangleShape::setPosition (double x, double y)
{
    sfml_rect.setPosition (x, y);
}

void RectangleShape::setScale (const plug::Vec2d factors)
{
    sfml_rect.setScale (getSFMLVector2f (factors));
}


plug::Vec2d RectangleShape::getSize () const
{
    sf::Vector2f size = sfml_rect.getSize ();
    return plug::Vec2d (size.x, size.y);
}

plug::Color RectangleShape::getFillColor () const
{
    sf::Color fill_color = sfml_rect.getFillColor ();
    return plug::Color (fill_color.r, fill_color.g, fill_color.b, fill_color.a);
}

plug::Color RectangleShape::getOutlineColor () const
{
    sf::Color outline_color = sfml_rect.getOutlineColor ();
    return plug::Color (outline_color.r, outline_color.g, outline_color.b, outline_color.a);
}

double RectangleShape::getOutlineThickness () const
{
    return sfml_rect.getOutlineThickness ();
}

plug::Vec2d RectangleShape::getPosition () const
{
    sf::Vector2f pos = sfml_rect.getPosition ();
    return plug::Vec2d (pos.x, pos.y);
}

plug::Vec2d RectangleShape::getScale () const
{
    sf::Vector2f scale = sfml_rect.getScale ();
    return plug::Vec2d (scale.x, scale.y);
}
