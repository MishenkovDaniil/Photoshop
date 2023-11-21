#include "vector.h"


inline plug::Vec2d plug::operator + (const plug::Vec2d &left, const plug::Vec2d &right)
{
    return plug::Vec2d (left.x + right.x, left.y + right.y);
}

inline plug::Vec2d plug::operator - (const plug::Vec2d &left, const plug::Vec2d &right)
{
    return plug::Vec2d (left.x - right.x, left.y - right.y);
} 

inline plug::Vec2d plug::operator / (const plug::Vec2d &vec, const double val)
{
    return plug::Vec2d (vec.x / val, vec.y / val);
}   

inline plug::Vec2d plug::operator - (const plug::Vec2d &vec)
{
    return plug::Vec2d (-vec.x, -vec.y);
}

inline plug::Vec2d plug::operator * (const plug::Vec2d &vec, const double val)
{
    return plug::Vec2d (vec.x * val, vec.y * val);
}

inline plug::Vec2d plug::operator * (const double val, const plug::Vec2d &vec)
{
    return vec * val;
}

inline plug::Vec2d plug::operator / (const plug::Vec2d &left, const plug::Vec2d &right)
{
    return plug::Vec2d (left.x / right.x, left.y / right.y);
}  

inline plug::Vec2d plug::operator * (const plug::Vec2d &left, const plug::Vec2d &right)
{
    return plug::Vec2d (left.x * right.x, left.y * right.y);
}

plug::Vec2d plug::Vec2d::operator =  (const plug::Vec2d &right)
{
    x = right.x;
    y = right.y;

    return *this;
}


inline plug::Vec2d& plug::operator *= (plug::Vec2d &vec,	 const double val)
{
    vec.x *= val;
    vec.y *= val;

    return vec;
} 

inline plug::Vec2d& plug::operator /= (plug::Vec2d &vec, const double val)
{
    vec.x /= val;
    vec.y /= val;

    return vec;
}

inline plug::Vec2d& plug::operator += (plug::Vec2d &left, const plug::Vec2d &right)
{
    left.x += right.x;
    left.y += right.y;

    return left;
}

inline plug::Vec2d& plug::operator -= (plug::Vec2d &left, const plug::Vec2d &right)
{
    left.x -= right.x;
    left.y -= right.y;

    return left;
}   

inline plug::Vec2d& plug::operator *= (plug::Vec2d &left, const plug::Vec2d &right)
{
    left.x *= right.x;
    left.y *= right.y;

    return left;
} 

inline plug::Vec2d& plug::operator /= (plug::Vec2d &left, const plug::Vec2d &right)
{
    left.x /= right.x;
    left.y /= right.y;

    return left;
} 


inline double plug::dot (plug::Vec2d &left, plug::Vec2d &right)
{
    return left.x * right.x + left.y * right.y;
}	

inline double plug::cross (plug::Vec2d &left, plug::Vec2d &right)
{
    return left.x * right.y - left.y * right.x;
} 

inline plug::Vec2d plug::normalize (const plug::Vec2d &vec)
{
    double module = sqrt (vec.x * vec.x + vec.y * vec.y);

    return plug::Vec2d (vec.x / module, vec.y / module);
}


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
