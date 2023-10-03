#include "color.h"

Color Color::operator * (double mult) const 
{
    double r = r_ * mult;
           r = r > 0xff ? 0xff : r;
    double g = g_ * mult;
           g = g > 0xff ? 0xff : g;
    double b = b_ * mult;
           b = b > 0xff ? 0xff : b;
    double a = a_ * mult;
           a = a > 0xff ? 0xff : a;        
    
    return Color (r, g, b, a);
}

Color Color::operator + (const Color &color) const
{
    double r = r_ + color.r_;
           r = r > 0xff ? 0xff : r;
    double g = g_ + color.g_;
           g = g > 0xff ? 0xff : g;
    double b = b_ + color.b_;
           b = b > 0xff ? 0xff : b;
    double a = a_ + color.a_;
           a = a > 0xff ? 0xff : a; 
            
    return Color (r, g, b, a);
}

Color Color::operator += (const Color &color)      
{
    r_ += color.r_;
    r_ = r_ < color.r_ ? 0xff : r_;
    g_ += color.g_;
    g_ = g_ < color.g_ ? 0xff : g_;
    b_ += color.b_;
    b_ = b_ < color.b_ ? 0xff : b_;
    a_ += color.a_;
    a_ = a_ < color.a_ ? 0xff : a_;
                                                        
    return Color (r_, g_, b_, a_);
}