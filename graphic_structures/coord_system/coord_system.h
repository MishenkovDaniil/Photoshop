#ifndef COORD_SYSTEM_H
#define COORD_SYSTEM_H

#include <SFML/Graphics.hpp>

class Vector;
class Point;
#include "../math_vector/math_vector.h"

class Coord_system
{
    int y_shift_ = 0;
    int x_shift_ = 0;
    int y_len_   = 0;
    int x_len_   = 0;
    int window_len_x = 0;
    int window_len_y = 0;
    int x_start_ = 0;
    int y_start_ = 0;

private:
    Math_vector *abscissa = nullptr;
    Math_vector *ordinate = nullptr;
    
public:
    friend void draw_line (const Math_vector &vec, const Coord_system &coord_system, sf::RenderTarget &target, const Point start);
    Coord_system (int x_start, int y_start, int x_len, int y_len, int x_shift, int y_shift, sf::VideoMode videomode);
    Coord_system () {;};
    ~Coord_system ();
    
    void start (Point &point);
    Coord_system operator = (const Coord_system &Coord_system);    
    void draw (sf::RenderTarget &target);
};

#endif /* COORD_SYSTEM_H */