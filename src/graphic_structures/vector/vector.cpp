#include "vector.h"

void plug::Vec2d::rotate (double deg)
{  
    double radians = deg * PI / 180.0;
    
    double deg_cos = cos (radians);
    double deg_sin = sin (radians);
    
    double x_new = x;
    double y_new = y;

    x = deg_cos * x_new - deg_sin * y_new; ///(cos_a + sin_a * i) * (x + y * i) = (cos_a * x - sin_a * y) + (y * cos_a + sin_a * x)
    y = deg_cos * y_new + deg_sin * x_new;
}

// // may be made base class for text and image to add it as parameter in button and use int it whether image or text
// //image of our size-->texture-->sprite-->sprite.set_texture () and sprite.set_texture_rect ()
