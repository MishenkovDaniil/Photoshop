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
    ~Drawable () = default;

    friend RenderTexture;
};


#endif /* DRAWABLE_H */