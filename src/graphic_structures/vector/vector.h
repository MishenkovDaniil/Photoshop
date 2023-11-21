#ifndef GRAPHIC_VECTOR_H
#define GRAPHIC_VECTOR_H

#include <cstring>
#include <cassert>
#include <iostream>
#include <cmath>
#include <cassert>
#include <SFML/Graphics.hpp>

#include "../point/point.h"

// static const int START_CAPACITY = 10;
// static const double LOAD_FACTOR = 0.7;
// static const double MULT_COEFF = 1.5;
static const double PI = 3.14;

//TODO:make on template + the same to point
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
        
        operator sf::Vector2f() const {return sf::Vector2f (x, y);};
        operator sf::Vector2i() const {return sf::Vector2i (x, y);};
        operator sf::Vector2u() const {return sf::Vector2u (x, y);};
    };


    inline Vec2d operator + (const Vec2d &left, const Vec2d &right)
    {
        return Vec2d (left.x + right.x, left.y + right.y);
    }

    inline Vec2d operator - (const Vec2d &left, const Vec2d &right)
    {
        return Vec2d (left.x - right.x, left.y - right.y);
    } 

    inline Vec2d operator / (const Vec2d &vec, const double val)
    {
        return Vec2d (vec.x / val, vec.y / val);
    }   

    inline Vec2d operator - (const Vec2d &vec)
    {
        return Vec2d (-vec.x, -vec.y);
    }

    inline Vec2d operator * (const Vec2d &vec, const double val)
    {
        return Vec2d (vec.x * val, vec.y * val);
    }

    inline Vec2d operator * (const double val, const Vec2d &vec)
    {
        return vec * val;
    }

    inline Vec2d operator / (const Vec2d &left, const Vec2d &right)
    {
        return Vec2d (left.x / right.x, left.y / right.y);
    }  

    inline Vec2d operator * (const Vec2d &left, const Vec2d &right)
    {
        return Vec2d (left.x * right.x, left.y * right.y);
    }

    inline Vec2d& operator *= (Vec2d &vec,	 const double val)
    {
        vec.x *= val;
        vec.y *= val;

        return vec;
    } 

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
    }

    inline Vec2d& operator -= (Vec2d &left, const Vec2d &right)
    {
        left.x -= right.x;
        left.y -= right.y;

        return left;
    }   

    inline Vec2d& operator *= (Vec2d &left, const Vec2d &right)
    {
        left.x *= right.x;
        left.y *= right.y;

        return left;
    } 

    inline Vec2d& operator /= (Vec2d &left, const Vec2d &right)
    {
        left.x /= right.x;
        left.y /= right.y;

        return left;
    } 
    inline double dot (Vec2d &left, Vec2d &right);	  // скалярное произведение
    inline double cross (Vec2d &left, Vec2d &right); // ориентированная площадь, порожденная векторами          
    inline Vec2d normalize (const Vec2d &vec);
}

#endif /* GRAPHIC_VECTOR_H */