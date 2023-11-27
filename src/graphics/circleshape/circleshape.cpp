#include "circleshape.h"
#include "../convert_sfml_functions.h"

CircleShape::CircleShape (double radius, size_t point_count)
    : Shape (static_cast<size_t>(Shapes::Circle))
      {
        drawable = new sf::CircleShape (radius, point_count);
      }

CircleShape::~CircleShape ()
{
    if (drawable) 
    {
        delete drawable;
        drawable = nullptr;
    }
}

void CircleShape::setRadius (double radius)
{
    ((sf::CircleShape *)drawable)->setRadius (radius);
}

void CircleShape::setFillColor (const plug::Color color)
{
    ((sf::CircleShape *)drawable)->setFillColor (getSFMLColor (color));
}

void CircleShape::setOutlineColor (const plug::Color color)
{
    ((sf::CircleShape *)drawable)->setOutlineColor (getSFMLColor (color));
}

void CircleShape::setOutlineThickness (double thickness)
{
    ((sf::CircleShape *)drawable)->setOutlineThickness (thickness);
}

void CircleShape::setPosition (double x, double y)
{
    ((sf::CircleShape *)drawable)->setPosition (x, y);
}

void CircleShape::setPosition (plug::Vec2d pos)
{
    ((sf::CircleShape *)drawable)->setPosition (getSFMLVector2f (pos)); 
}

void CircleShape::setScale (const plug::Vec2d factors)
{
    ((sf::CircleShape *)drawable)->setScale (getSFMLVector2f (factors));
}

void CircleShape::setPointCount (size_t point_count)
{
    ((sf::CircleShape *)drawable)->setPointCount (point_count);
}

double CircleShape::getRadius () const
{
    return ((sf::CircleShape *)drawable)->getRadius ();
}

plug::Color CircleShape::getFillColor () const
{
    sf::Color fill_color = ((sf::CircleShape *)drawable)->getFillColor ();
    return plug::Color (fill_color.r, fill_color.g, fill_color.b, fill_color.a);
}

plug::Color CircleShape::getOutlineColor () const
{
    sf::Color outline_color = ((sf::CircleShape *)drawable)->getOutlineColor ();
    return plug::Color (outline_color.r, outline_color.g, outline_color.b, outline_color.a);
}

double CircleShape::getOutlineThickness () const
{
    return ((sf::CircleShape *)drawable)->getOutlineThickness ();
}

plug::Vec2d CircleShape::getPosition () const
{
    sf::Vector2f pos = ((sf::CircleShape *)drawable)->getPosition ();
    return plug::Vec2d (pos.x, pos.y);
}

plug::Vec2d CircleShape::getScale () const
{
    sf::Vector2f scale = ((sf::CircleShape *)drawable)->getScale ();
    return plug::Vec2d (scale.x, scale.y);
}

size_t CircleShape::getPointCount () const
{
    return ((sf::CircleShape *)drawable)->getPointCount ();
}

