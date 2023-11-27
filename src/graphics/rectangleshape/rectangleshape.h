#ifndef RECTANGLESHAPE_H
#define RECTANGLESHAPE_H

#include <SFML/Graphics.hpp>

#include "../../graphic_structures/vector/vector.h"
#include "../../graphic_structures/color/color.h"
#include "../shape.h"

class RectangleShape : public Shape
{
public:
    // sf::RectangleShape sfml_rect;
    RectangleShape (const plug::Vec2d &size = plug::Vec2d (0, 0));
    ~RectangleShape ();

    void setSize (const plug::Vec2d &size);
    void setFillColor (const plug::Color color);
    void setOutlineColor (const plug::Color color);
    void setOutlineThickness (double thickness);
    void setPosition (double x, double y);
    void setPosition (plug::Vec2d pos);
    void setScale (const plug::Vec2d factors);

    plug::Vec2d getSize () const;
    plug::Color getFillColor () const;
    plug::Color getOutlineColor () const;
    double getOutlineThickness () const;
    plug::Vec2d getPosition () const;
    plug::Vec2d getScale () const;
};


#endif /* RECTANGLESHAPE_H */