#ifndef HSL_LUMA_COLOR
#define HSL_LUMA_COLOR

#include <cmath>
#include "color.h"

static const double LUMA_R_PARAM = 0.299;
static const double LUMA_G_PARAM = 0.587;
static const double LUMA_B_PARAM = 0.114;

static const double MAX_COLOR_VAL = 255.0;
static const double MIN_COLOR_VAL = 0.0;

class Luma_color 
{
public: 
    double hue_ = 0;
    double luma_ = 0;
    double chroma_ = 0;
    double saturation_ = 0;

    Luma_color (double hue, double chroma, double luma) : hue_ (hue), luma_ (luma), chroma_ (chroma) {};
    ~Luma_color () = default;
};

class Hsl_color 
{
public: 
    double hue_ = 0;
    double light_ = 0;
    double saturation_ = 0;

    Hsl_color (double hue, double saturation, double light) : hue_ (hue), light_ (light), saturation_ (saturation) {};
    ~Hsl_color () = default;
};


Luma_color rgb_to_luma (const plug::Color &color);
plug::Color luma_to_rgb (Luma_color &luma_color);
Hsl_color  rgb_to_hsl  (const plug::Color &color);
plug::Color hsl_to_rgb (Hsl_color &hsl_color);

#endif /* HSL_LUMA_COLOR */