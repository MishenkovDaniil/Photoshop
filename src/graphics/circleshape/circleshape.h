#ifndef CIRCLESHAPE_H
#define CIRCLESHAPE_H

#include <SFML/Graphics.hpp>

#include "../../graphic_structures/vector/vector.h"
#include "../../graphic_structures/color/color.h"
#include "../shape.h"


class CircleShape : public Shape
{   
public:
    CircleShape (double radius = 0, size_t point_count = 30);
    ~CircleShape ();

    void setRadius (double radius);
    void setFillColor (const plug::Color color);
    void setOutlineColor (const plug::Color color);
    void setOutlineThickness (double thickness);
    void setPosition (double x, double y);
    void setPosition (plug::Vec2d pos);
    void setScale (const plug::Vec2d factors);
    void setPointCount (size_t point_count);
    CircleShape *clone () const override;

    double getRadius () const;
    plug::Color getFillColor () const;
    plug::Color getOutlineColor () const;
    double getOutlineThickness () const;
    plug::Vec2d getPosition () const;
    plug::Vec2d getScale () const;
    size_t getPointCount () const;
};


#endif /* CIRCLESHAPE_H */