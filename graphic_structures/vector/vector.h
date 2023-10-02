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

class Vector 
{
    double x_ = 0;
    double y_ = 0;
    double z_ = 0;

public:
    Vector (double x = 0, double y = 0, double z = 0) : x_ (x), y_ (y), z_ (z) {};
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
    double get_x (){return x_;};
    double get_y (){return y_;};
    double get_z (){return z_;};
    operator Point() {return Point (x_, y_, z_);};
};

#endif /* GRAPHIC_VECTOR_H */