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


Color Color::operator = (const sf::Color &other)
{
       r_ = other.r;
       g_ = other.g;
       b_ = other.b;
       a_ = other.a;

       return *this;
}


Luma_color rgb_to_luma (const Color &color)
{
       double r = (double)color.r_ / MAX_COLOR_VAL;
       double g = (double)color.g_ / MAX_COLOR_VAL;
       double b = (double)color.b_ / MAX_COLOR_VAL;
       
       double luma = LUMA_R_PARAM * r + LUMA_G_PARAM * g + LUMA_B_PARAM * b;

       double max = std::max (std::max (r, g), b);
       double min = std::min (std::min (r, g), b);
       double chroma_ = max - min;
     
       double hue = 0;
       if (chroma_)
       {
              if (!(max - r))
              {
                     double segment  = ((g - b) / chroma_);
                     double shift = 0;
                     while (segment > 6)
                     {
                            segment -= 6.0;
                     }
                     hue = 60 * (segment + shift);
              }
              else if (!(max - g))
              {
                     double segment  = ((b - r) / chroma_);
                     double shift = 2;
                     hue = 60 * (segment + shift); 
              }
              else 
              {
                     double segment  = ((r - g) / chroma_);
                     double shift = 4;
                     hue = 60 * (segment + shift);  
              }
       }
       return Luma_color (hue, chroma_, luma);
}

Color luma_to_rgb (Luma_color &luma_color)
{
       double h_ = luma_color.hue_ / 60.0;
       double h_temp = h_;

       while (h_temp > 2)
       {
              h_temp -= 2;
       }
       double x = luma_color.chroma_ * (1 - std::abs (h_temp - 1));

       double r = 0;
       double g = 0;
       double b = 0;

       int int_h = round (h_ - 0.5);
       
       switch (int_h)
       {
              case 0:
              {
                     r = luma_color.chroma_;
                     g = x;
              }  
              case 1:
              {
                     r = x;
                     g = luma_color.chroma_;
              }  
              case 2:
              {
                     g = luma_color.chroma_;
                     b = x;
              }  
              case 3:
              {
                     g = x;
                     b = luma_color.chroma_;
              }  
              case 4:
              {
                     r = x;
                     b = luma_color.chroma_;
              }  
              default:
              {
                     r = luma_color.chroma_;
                     b = x;  
              }
       }

       double m = luma_color.luma_ - (LUMA_R_PARAM * r + LUMA_G_PARAM * g + LUMA_B_PARAM * b);

       r += m;
       g += m;
       b += m;

       r = std::max (MIN_COLOR_VAL, r);
       g = std::max (MIN_COLOR_VAL, g);
       b = std::max (MIN_COLOR_VAL, b);

       r = std::min (MAX_COLOR_VAL, r * MAX_COLOR_VAL);
       g = std::min (MAX_COLOR_VAL, g * MAX_COLOR_VAL);
       b = std::min (MAX_COLOR_VAL, b * MAX_COLOR_VAL);

       return Color (r, g, b);
}