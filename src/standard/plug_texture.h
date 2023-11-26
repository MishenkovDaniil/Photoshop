#ifndef PLUG_TEXTURE_H
#define PLUG_TEXTURE_H

#include <iostream>

#include "../graphic_structures/color/color.h"

namespace plug
{
    struct Texture
    {
        Color* data = nullptr;
        const size_t width;
        const size_t height;

        Texture (size_t width, size_t height, Color* const data) :
            data(data), width(width), height(height) {};

        Texture (size_t width, size_t height) :
            data((Color*)new Color[width * height]), width(width), height(height) {};           
        
        ~Texture () {if (data)
                    {
                        delete[] data;
                        data = nullptr;
                    }};
    };
}

#endif /* PLUG_TEXTURE_H */