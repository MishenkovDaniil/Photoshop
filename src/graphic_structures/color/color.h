#ifndef COLOR_H
#define COLOR_H

#include <cmath>
#include <iostream>

namespace plug
{
    class Color 
    {
    public:
    Color(void) : Color(255, 255, 255, 255) {}

    Color(uint8_t red_, uint8_t green_, uint8_t blue_, uint8_t alpha_ = 255)
        : red(red_), green(green_), blue(blue_), alpha(alpha_) {}

    union {
        uint8_t red; /*!< Red channel value 0 to 255 inclusive */
        uint8_t r;   /*!< Red channer value 0 to 255 inclusive */
    };

    union {
        uint8_t green; /*!< Green channel value 0 to 255 inclusive */
        uint8_t g;     /*!< Green channel value 0 to 255 inclusive */
    };

    union {
        uint8_t blue; /*!< Blue channel value 0 to 255 inclusive */
        uint8_t b;    /*!< Blue channel value 0 to 255 inclusive */
    };

    union {
        uint8_t alpha; /*!< Alpha channel value 0 to 255 inclusive */
        uint8_t a;     /*!< Alpha channel value 0 to 255 inclusive */
    };
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