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
class Vector 
{
    double x_ = 0;
    double y_ = 0;
    double z_ = 0;

public:
    Vector (double x = 0, double y = 0, double z = 0) : x_ (x), y_ (y), z_ (z) {};
    Vector (const Vector &other) : x_ (other.x_), y_ (other.y_), z_ (other.z_) {};
    Vector (const Point &start, const Point &end) : x_ (end.x_ - start.x_),
                                                    y_ (end.y_ - start.y_), 
                                                    z_ (end.z_ - start.z_) {};

    Point vec_to_point ();
    void rotate (double deg);
    Vector operator *  (const Vector &vec)const; //векторное произведение
    Vector operator +  (const Vector &vec)const;
    Vector operator += (const Vector &vec);
    Vector operator -= (const Vector &vec);
    Vector operator -  (const Vector &vec)const;
    Vector operator -  ()const;
    Vector operator && (const double mult)const;  // покоординатное умножение
    Vector operator !  () const;                  // нормализация вектора
    double operator || (const Vector &vec)const ; // скалярное умножение
    double get_x ()const {return x_;};
    double get_y ()const {return y_;};
    double get_z ()const {return z_;};
    operator Point()        const {return Point        (x_, y_, z_);  };
    operator sf::Vector3f() const {return sf::Vector3f (x_, y_, z_);  };
    operator sf::Vector2f() const {return sf::Vector2f (x_, y_);      };
    operator sf::Vector2i() const {return sf::Vector2i (x_, y_);      };
    operator sf::Vector2u() const {return sf::Vector2u (x_, y_);      };
};

#endif /* GRAPHIC_VECTOR_H */