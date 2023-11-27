#include "rectangleshape.h"
#include "../convert_sfml_functions.h"

RectangleShape::RectangleShape (const plug::Vec2d &size)
    : Shape (static_cast<size_t>(Shapes::Rectangle))
      {
        drawable = new sf::RectangleShape (getSFMLVector2f (size));
      }

RectangleShape::~RectangleShape ()
{
    if (drawable) 
    {
        delete drawable;
        drawable = nullptr;
    }
}


void RectangleShape::setSize (const plug::Vec2d &size)
{
    ((sf::RectangleShape *)drawable)->setSize (getSFMLVector2f (size));
}

void RectangleShape::setFillColor (const plug::Color color)
{
    ((sf::RectangleShape *)drawable)->setFillColor (getSFMLColor (color));
}

void RectangleShape::setOutlineColor (const plug::Color color)
{
    ((sf::RectangleShape *)drawable)->setOutlineColor (getSFMLColor (color));
}

void RectangleShape::setOutlineThickness (double thickness)
{
    ((sf::RectangleShape *)drawable)->setOutlineThickness (thickness);
}

void RectangleShape::setPosition (double x, double y)
{
    ((sf::RectangleShape *)drawable)->setPosition (x, y);
}

void RectangleShape::setPosition (plug::Vec2d pos)
{
    ((sf::RectangleShape *)drawable)->setPosition (getSFMLVector2f (pos)); 
}

void RectangleShape::setScale (const plug::Vec2d factors)
{
    ((sf::RectangleShape *)drawable)->setScale (getSFMLVector2f (factors));
}


plug::Vec2d RectangleShape::getSize () const
{
    sf::Vector2f size = ((sf::RectangleShape *)drawable)->getSize ();
    return plug::Vec2d (size.x, size.y);
}

plug::Color RectangleShape::getFillColor () const
{
    sf::Color fill_color = ((sf::RectangleShape *)drawable)->getFillColor ();
    return plug::Color (fill_color.r, fill_color.g, fill_color.b, fill_color.a);
}

plug::Color RectangleShape::getOutlineColor () const
{
    sf::Color outline_color = ((sf::RectangleShape *)drawable)->getOutlineColor ();
    return plug::Color (outline_color.r, outline_color.g, outline_color.b, outline_color.a);
}

double RectangleShape::getOutlineThickness () const
{
    return ((sf::RectangleShape *)drawable)->getOutlineThickness ();
}

plug::Vec2d RectangleShape::getPosition () const
{
    sf::Vector2f pos = ((sf::RectangleShape *)drawable)->getPosition ();
    return plug::Vec2d (pos.x, pos.y);
}

plug::Vec2d RectangleShape::getScale () const
{
    sf::Vector2f scale = ((sf::RectangleShape *)drawable)->getScale ();
    return plug::Vec2d (scale.x, scale.y);
}
