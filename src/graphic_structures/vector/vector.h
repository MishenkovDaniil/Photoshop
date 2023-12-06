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
    class Vec2d 
    {
    public:
        explicit Vec2d(const double x_ = 0.0, const double y_ = 0.0) : x(x_), y(y_) {};

        Vec2d(const Vec2d &src) = default;

        ~Vec2d() = default;

        double length2() const { return x * x + y * y; }

        double length() const { return sqrt(this->length2()); }
        void  rotate (double deg);
        double get_x () const {return x;};
        double get_y () const {return y;};
        
        Vec2d &operator=(const Vec2d &vec) 
        {
            x = vec.x;
            y = vec.y;

            return *this;
        }

        Vec2d &operator+=(const Vec2d &vec) 
        {
            x += vec.x;
            y += vec.y;

            return *this;
        }

        Vec2d &operator-=(const Vec2d &vec) 
        {
            x -= vec.x;
            y -= vec.y;

            return *this;
        }

        Vec2d &operator*=(const Vec2d &vec) 
        {
            x *= vec.x;
            y *= vec.y;

            return *this;
        }

        Vec2d &operator/=(const Vec2d &vec) 
        {
            x /= vec.x;
            y /= vec.y;

            return *this;
        }

        Vec2d &operator*=(const double val) 
        {
            x *= val;
            y *= val;

            return *this;
        }

        Vec2d &operator/=(const double val) 
        {
            x /= val;
            y /= val;

            return *this;
        }

        Vec2d operator -() const
        {
            Vec2d res;
            res.x = -x;
            res.y = -y;

            return res;
        }

        double x;
        double y;
    };

    inline double dot(const Vec2d &lhs, const Vec2d &rhs) 
    {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    inline double cross(const Vec2d &lhs, const Vec2d &rhs) 
    {
        return lhs.x * rhs.y - lhs.y * rhs.x;
    }

    inline Vec2d normalize(const Vec2d &vec) 
    {
        double len = vec.length();
        Vec2d res = Vec2d(vec.x / len, vec.y / len);

        return res;
    }

    inline Vec2d operator+(const Vec2d &vec1, const Vec2d &vec2) 
    {
        Vec2d res = vec1;
        res += vec2;

        return res;
    }

    inline Vec2d operator-(const Vec2d &vec1, const Vec2d &vec2) 
    {
        Vec2d res = vec1;
        res -= vec2;

        return res;
    }

    inline Vec2d operator*(const Vec2d &lhs, const Vec2d &rhs) 
    {
        Vec2d res = lhs;
        res *= rhs;

        return res;
    }

    inline Vec2d operator/(const Vec2d &lhs, const Vec2d &rhs) 
    {
        Vec2d res = lhs;
        res /= rhs;

        return res;
    }

    inline Vec2d operator*(const Vec2d &vec, const double scale) 
    {
        Vec2d res = vec;
        res *= scale;

        return res;
    }

    inline Vec2d operator*(const double scale, const Vec2d &vec) 
    {
        return operator*(vec, scale);
    }

    inline Vec2d operator/(const Vec2d &vec, const double scale) 
    {
        Vec2d res = vec;
        res /= scale;

        return res;
    }
} // namespace plug

#endif /* GRAPHIC_VECTOR_H */