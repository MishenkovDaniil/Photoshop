#ifndef COLOR_H
#define COLOR_H

#include <cmath>
#include <iostream>

namespace plug
{
    class Color
    {
    public:
        union
        {
            uint8_t r = 0;
            uint8_t red;
        };
        union
        {
            uint8_t g = 0;
            uint8_t green;
        };
        union
        {
            uint8_t b = 0;
            uint8_t blue;
        };
        union
        {
            uint8_t a = 0;
            uint8_t alpha;
        };

    public:
        Color () {};
        Color (uint8_t r_val, uint8_t g_val, uint8_t b_val, uint8_t a_val = 0xff) : r (r_val), g (g_val), b (b_val), a (a_val) {};

        Color operator *  (double mult) const;
        Color operator +  (const Color &color) const;
        Color operator += (const Color &color);
        Color operator = (const Color &color);
    };

    static const Color Transparent  = Color (0, 0, 0, 0);
    static const Color Red          = Color (255, 0, 0);
    static const Color Green        = Color (0, 255, 0);
    static const Color Blue         = Color (0, 0, 255);
    static const Color White        = Color (255, 255, 255);
    static const Color Black        = Color (0, 0, 0);
    static const Color Cyan         = Color (0, 255, 255);
    static const Color Purple       = Color (255, 0, 255);
    static const Color Yellow       = Color (255, 255, 0);
    static const Color Brown        = Color (165, 42, 42);
    static const Color Maroon       = Color (128, 0, 0);
}

#endif /* COLOR_H */