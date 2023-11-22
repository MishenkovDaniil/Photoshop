#ifndef PLUG_COLORPALETTE_H
#define PLUG_COLORPALETTE_H

#include "../graphic_structures/color/color.h"

namespace plug 
{
    class ColorPalette 
    {
        Color fg_color_ = Red;
        Color bg_color_ = White;
    public:
        Color getFGColor () const {return fg_color_;}; 
        Color getBGColor () const {return bg_color_;}; 
        void setFGColor (Color fg_color) {fg_color_ = fg_color;};
        void setBGColor (Color bg_color) {bg_color_ = bg_color;};
    };
}

#endif /* PLUG_COLORPALETTE_H */