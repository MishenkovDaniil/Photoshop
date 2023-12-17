#ifndef SHAPE_H
#define SHAPE_H

#include "../graphic_structures/vector/vector.h"
#include "../graphic_structures/color/color.h"

#include "drawable.h"

class RenderTexture;

enum class Shapes
{
    Rectangle,
    Circle
};

class Shape : public Drawable
{
    size_t shape;
public:
    Shape (size_t shape_type) : shape (shape_type) {};
    virtual ~Shape () = default;

    size_t getType () const {return shape;};
    virtual void setFillColor (const plug::Color color) = 0;
    virtual void setOutlineColor (const plug::Color color) = 0;
    virtual void setOutlineThickness (double thickness) = 0;
    virtual void setScale (const plug::Vec2d factors) = 0;

    Shape *clone () const override = 0;

    virtual plug::Color getFillColor () const = 0;
    virtual plug::Color getOutlineColor () const = 0;
    virtual double getOutlineThickness () const = 0;
    virtual plug::Vec2d getScale () const = 0;

    friend RenderTexture;
};


#endif /* SHAPE_H */