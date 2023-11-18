#ifndef COLOR_H
#define COLOR_H

#include <SFML/Graphics.hpp>
#include <cmath>
class Luma_color;
class Color;
class Hsl_color;

static const double LUMA_R_PARAM = 0.299;
static const double LUMA_G_PARAM = 0.587;
static const double LUMA_B_PARAM = 0.114;

static const double MAX_COLOR_VAL = 255.0;
static const double MIN_COLOR_VAL = 0.0;

Luma_color rgb_to_luma (const Color &color);
Color luma_to_rgb (Luma_color &luma_color);
Hsl_color  rgb_to_hsl  (const Color &color);
Color hsl_to_rgb (Hsl_color &hsl_color);

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
    Color () {;};
    Color (uint8_t r_val, uint8_t g_val, uint8_t b_val, uint8_t a_val = 0xff) : r (r_val), g (g_val), b (b_val), a (a_val) {};

    Color operator *  (double mult) const;
    Color operator +  (const Color &color) const;
    Color operator += (const Color &color);
    Color operator = (const sf::Color &color);
    operator sf::Color()const {return sf::Color (r, g, b, a);};

    friend Luma_color rgb_to_luma (const Color &color);
    friend Hsl_color  rgb_to_hsl  (const Color &color);
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


class Luma_color 
{
public: 
    double hue_ = 0;
    double luma_ = 0;
    double chroma_ = 0;
    double saturation_ = 0;

    Luma_color (double hue, double chroma, double luma) : hue_ (hue), luma_ (luma), chroma_ (chroma) {};
    ~Luma_color () = default;
    friend Color luma_to_rgb (Luma_color &hsl_color);
};

class Hsl_color 
{
public: 
    double hue_ = 0;
    double light_ = 0;
    double saturation_ = 0;

    Hsl_color (double hue, double saturation, double light) : hue_ (hue), light_ (light), saturation_ (saturation) {};
    ~Hsl_color () = default;
    friend Color hsl_to_rgb (Hsl_color &hsl_color);
};

#endif /* COLOR_H */