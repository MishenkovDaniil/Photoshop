#ifndef COLOR_H
#define COLOR_H

#include <SFML/Graphics.hpp>

class Color
{
public:
    uint8_t r_ = 0;
    uint8_t g_ = 0;
    uint8_t b_ = 0;
    uint8_t a_ = 0;

public:
    Color () {;};
    Color (uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff) : r_(r), g_ (g), b_ (b), a_(a) {};

    Color operator *  (double mult) const;
    Color operator +  (const Color &color) const;
    Color operator += (const Color &color);
    operator sf::Color()const {return sf::Color (r_, g_, b_, a_);};
};

#endif /* COLOR_H */