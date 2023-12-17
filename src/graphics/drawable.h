#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "../graphic_structures/vector/vector.h"
#include "../graphic_structures/color/color.h"

class RenderTexture;


class Drawable
{
protected:
public:
    sf::Drawable *drawable;
    virtual ~Drawable () = default;

    virtual Drawable *clone () const = 0;
    virtual void setPosition (const plug::Vec2d pos) = 0;
    virtual void setPosition (double x, double y) = 0;
    virtual plug::Vec2d getPosition () const = 0;

    friend RenderTexture;
};


#endif /* DRAWABLE_H */