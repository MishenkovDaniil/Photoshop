#include "hsl_luma_color.h"

Luma_color rgb_to_luma (const plug::Color &color)
{
       double r = (double)color.r / MAX_COLOR_VAL;
       double g = (double)color.g / MAX_COLOR_VAL;
       double b = (double)color.b / MAX_COLOR_VAL;
       
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

plug::Color luma_to_rgb (Luma_color &luma_color)
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
                     break;
              }  
              case 1:
              {
                     r = x;
                     g = luma_color.chroma_;
                     break;
              }  
              case 2:
              {
                     g = luma_color.chroma_;
                     b = x;
                     break;
              }  
              case 3:
              {
                     g = x;
                     b = luma_color.chroma_;
                     break;
              }  
              case 4:
              {
                     r = x;
                     b = luma_color.chroma_;
                     break;
              }  
              default:
              {
                     r = luma_color.chroma_;
                     b = x;  
                     break;
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

       return plug::Color (r, g, b);
}


Hsl_color  rgb_to_hsl  (const plug::Color &color)
{
       double r = (double)color.r / MAX_COLOR_VAL;
       double g = (double)color.g / MAX_COLOR_VAL;
       double b = (double)color.b / MAX_COLOR_VAL;
       

       double max = std::max (std::max (r, g), b);
       double min = std::min (std::min (r, g), b);
       double light = (max + min) * 0.5;
       double chroma_ = max - min;
     
       double hue = 0;
       double saturation = chroma_ / (1.0 - std::abs(2.0 * light - 1.0));

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
       return Hsl_color (hue, saturation, light);
}

plug::Color hsl_to_rgb (Hsl_color &hsl_color)
{
       double chroma = (1 - std::abs (2.0 * hsl_color.light_ - 1.0)) * hsl_color.saturation_;

       double h_ = hsl_color.hue_ / 60.0;
       double h_temp = h_;

       while (h_temp > 2)
       {
              h_temp -= 2;
       }
       double x = chroma * (1 - std::abs (h_temp - 1));

       double r = 0;
       double g = 0;
       double b = 0;

       int int_h = round (h_ - 0.5);

       switch (int_h)
       {
              case 0:
              {
                     r = chroma;
                     g = x;
                     break;
              }  
              case 1:
              {
                     r = x;
                     g = chroma;
                     break;
              }  
              case 2:
              {
                     g = chroma;
                     b = x;
                     break;
              }  
              case 3:
              {
                     g = x;
                     b = chroma;
                     break;
              }  
              case 4:
              {
                     r = x;
                     b = chroma;
                     break;
              }  
              default:
              {
                     r = chroma;
                     b = x;  
                     break;
              }
       }

       double m = hsl_color.light_ - chroma / 2.0;

       r += m;
       g += m;
       b += m;

       r = std::max (MIN_COLOR_VAL, r);
       g = std::max (MIN_COLOR_VAL, g);
       b = std::max (MIN_COLOR_VAL, b);

       r = std::min (MAX_COLOR_VAL, r * MAX_COLOR_VAL);
       g = std::min (MAX_COLOR_VAL, g * MAX_COLOR_VAL);
       b = std::min (MAX_COLOR_VAL, b * MAX_COLOR_VAL);

       return plug::Color (r, g, b);
}