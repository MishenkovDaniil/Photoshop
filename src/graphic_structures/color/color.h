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
    Color operator = (const sf::Color &color);
    operator sf::Color()const {return sf::Color (r_, g_, b_, a_);};
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

#endif /* COLOR_H */