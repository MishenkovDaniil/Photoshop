#include "circleshape.h"

CircleShape::CircleShape (double radius = 0, size_t point_count = 30)
    : Shape (static_cast<size_t>(Shapes::Circle)),
      sfml_circle(radius, point_count) {}


void CircleShape::setRadius (double radius)
{
    sfml_circle.setRadius (radius);
}

void CircleShape::setFillColor (const plug::Color color)
{
    sfml_circle.setFillColor (getSFMLColor (color));
}

void CircleShape::setOutlineColor (const plug::Color color)
{
    sfml_circle.setOutlineColor (getSFMLColor (color));
}

void CircleShape::setOutlineThickness (double thickness)
{
    sfml_circle.setOutlineThickness (thickness);
}

void CircleShape::setPosition (double x, double y)
{
    sfml_circle.setPosition (x, y);
}

void CircleShape::setScale (const plug::Vec2d factors)
{
    sfml_circle.setScale (getSFMLVector2f (factors));
}

void CircleShape::setPointCount (size_t point_count)
{
    sfml_circle.setPointCount (point_count);
}

double CircleShape::getRadius () const
{
    return sfml_circle.getRadius ();
}

plug::Color CircleShape::getFillColor () const
{
    sf::Color fill_color = sfml_circle.getFillColor ();
    return plug::Color (fill_color.r, fill_color.g, fill_color.b, fill_color.a);
}

plug::Color CircleShape::getOutlineColor () const
{
    sf::Color outline_color = sfml_circle.getOutlineColor ();
    return plug::Color (outline_color.r, outline_color.g, outline_color.b, outline_color.a);
}

double CircleShape::getOutlineThickness () const
{
    return sfml_circle.getOutlineThickness ();
}

plug::Vec2d CircleShape::getPosition () const
{
    sf::Vector2f pos = sfml_circle.getPosition ();
    return plug::Vec2d (pos.x, pos.y);
}

plug::Vec2d CircleShape::getScale () const
{
    sf::Vector2f scale = sfml_circle.getScale ();
    return plug::Vec2d (scale.x, scale.y);
}

size_t CircleShape::getPointCount () const
{
    return sfml_circle.getPointCount ();
}

