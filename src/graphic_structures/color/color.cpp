#include "color.h"

plug::Color plug::Color::operator * (double mult) const 
{
    double r_new = r * mult;
           r_new = r_new > 0xff ? 0xff : r_new;
    double g_new = g * mult;
           g_new = g_new > 0xff ? 0xff : g_new;
    double b_new = b * mult;
           b_new = b_new > 0xff ? 0xff : b_new;
    double a_new = a * mult;
           a_new = a_new > 0xff ? 0xff : a_new;        
    
    return plug::Color (r_new, g_new, b_new, a_new);
}

plug::Color plug::Color::operator + (const plug::Color &color) const
{
    double r_new = r + color.r;
           r_new = r_new > 0xff ? 0xff : r_new;
    double g_new = g + color.g;
           g_new = g_new > 0xff ? 0xff : g_new;
    double b_new = b + color.b;
           b_new = b_new > 0xff ? 0xff : b_new;
    double a_new = a + color.a;
           a_new = a_new > 0xff ? 0xff : a_new; 
            
    return plug::Color (r_new, g_new, b_new, a_new);
}

plug::Color plug::Color::operator += (const plug::Color &color)      
{
    r += color.r;
    r = r < color.r ? 0xff : r;
    g += color.g;
    g = g < color.g ? 0xff : g;
    b += color.b;
    b = b < color.b ? 0xff : b;
    a += color.a;
    a = a < color.a ? 0xff : a;
                                                        
    return plug::Color (r, g, b, a);
}

plug::Color plug::Color::operator = (const plug::Color &other)
{
       r = other.r;
       g = other.g;
       b = other.b;
       a = other.a;

       return *this;   
}
