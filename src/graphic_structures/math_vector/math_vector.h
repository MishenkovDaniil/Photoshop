#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H 

#include <cmath>
#include <SFML/Graphics.hpp>

class Coord_system;
class Vector;
// void draw_line (const Vector &vec, const Coord_system &coord_system, sf::RenderWindow &window, const Point start);
#include "../point/point.h"
#include "../coord_system/coord_system.h"

class Math_vector 
{
    double x_coord = 0;
    double y_coord = 0;
    sf::Color color_ = sf::Color ();

public:
    Math_vector (double x, double y, sf::Color color);
    ~Math_vector ();

    void draw (const Coord_system &coord_system, sf::RenderTarget &target, const Point start = Point (0, 0));
    Math_vector operator + (const Math_vector &vec_a);
    Math_vector operator - (const Math_vector &vec_a);
    Math_vector operator = (const Math_vector &vec_a);
    Math_vector operator - ();
    friend void draw_line (const Math_vector &vec, const Coord_system &coord_system, sf::RenderTarget &target, const Point start);
private:
};



#endif /* MATH_VECTOR_H */  