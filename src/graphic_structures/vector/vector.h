#ifndef GRAPHIC_VECTOR_H
#define GRAPHIC_VECTOR_H

#include <cstring>
#include <cassert>
#include <iostream>
#include <cmath>
#include <cassert>

static const double PI = 3.14;

namespace plug
{
    struct Vec2d 
    {
        double x = 0;
        double y = 0;

        Vec2d (double x_val = 0, double y_val = 0) : x (x_val), y (y_val) {};
        Vec2d (const Vec2d &other) : x (other.x), y (other.y) {};

        void rotate (double deg);

        double length  () const {return sqrt (length2 ());};
        double length2 () const {return x * x + y * y;};

        double get_x ()const {return x;};
        double get_y ()const {return y;};
        
        Vec2d  operator = (const Vec2d &right) 
            {
                x = right.x;
                y = right.y;

                return *this;
            }
    };


    inline Vec2d operator + (const Vec2d &left, const Vec2d &right)
    {
        return Vec2d (left.x + right.x, left.y + right.y);
    };

    inline Vec2d operator - (const Vec2d &left, const Vec2d &right)
    {
        return Vec2d (left.x - right.x, left.y - right.y);
    }; 

    inline Vec2d operator / (const Vec2d &vec, const double val)
    {
        return Vec2d (vec.x / val, vec.y / val);
    };   

    inline Vec2d operator - (const Vec2d &vec)
    {
        return Vec2d (-vec.x, -vec.y);
    };

    inline Vec2d operator * (const Vec2d &vec, const double val)
    {
        return Vec2d (vec.x * val, vec.y * val);
    };

    inline Vec2d operator * (const double val, const Vec2d &vec)
    {
        return vec * val;
    };

    inline Vec2d operator / (const Vec2d &left, const Vec2d &right)
    {
        return Vec2d (left.x / right.x, left.y / right.y);
    };  

    inline Vec2d operator * (const Vec2d &left, const Vec2d &right)
    {
        return Vec2d (left.x * right.x, left.y * right.y);
    };

    inline Vec2d& operator *= (Vec2d &vec,	 const double val)
    {
        vec.x *= val;
        vec.y *= val;

        return vec;
    }; 

    inline Vec2d& operator /= (Vec2d &vec, const double val)
    {
        vec.x /= val;
        vec.y /= val;

        return vec;
    }

    inline Vec2d& operator += (Vec2d &left, const Vec2d &right)
    {
        left.x += right.x;
        left.y += right.y;

        return left;
    };

    inline Vec2d& operator -= (Vec2d &left, const Vec2d &right)
    {
        left.x -= right.x;
        left.y -= right.y;

        return left;
    };  

    inline Vec2d& operator *= (Vec2d &left, const Vec2d &right)
    {
        left.x *= right.x;
        left.y *= right.y;

        return left;
    }; 

    inline Vec2d& operator /= (Vec2d &left, const Vec2d &right)
    {
        left.x /= right.x;
        left.y /= right.y;

        return left;
    }; 
    inline double dot (Vec2d &left, Vec2d &right)	  // скалярное произведение
    {
        return left.x * right.x + left.y * right.y;
    };
    inline double cross (Vec2d &left, Vec2d &right) // ориентированная площадь, порожденная векторами          
    {
        return left.x * right.y - left.y * right.x;
    };
    inline Vec2d normalize (const Vec2d &vec)
    {
        double module = sqrt (vec.x * vec.x + vec.y * vec.y);
        printf ("module = %lf\n", module);
        printf ("x = %lf\n", vec.x / module);
        printf ("y = %lf\n", vec.y / module);

        return plug::Vec2d (vec.x / module, vec.y / module);
    };
}

#endif /* GRAPHIC_VECTOR_H */