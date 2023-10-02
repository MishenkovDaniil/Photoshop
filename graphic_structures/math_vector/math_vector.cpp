#include <cmath>
#include <iostream>

#include "../coord_system/coord_system.h"
#include "math_vector.h"
#include "../point/point.h"

Math_vector::Math_vector (double x, double y, sf::Color color) :
    x_coord (x), 
    y_coord (y),
    color_  (color)
    {};

Math_vector::~Math_vector ()
{
    this->x_coord = NAN;
    this->y_coord = NAN;  
};

Math_vector Math_vector::operator + (const Math_vector &vec_a)
{
    sf::Color new_color ((this->color_.r + vec_a.color_.r) / 2, (this->color_.g + vec_a.color_.g) / 2,
                        (this->color_.b + vec_a.color_.b) / 2,(this->color_.a + vec_a.color_.a) / 2);
    return Math_vector (this->x_coord + vec_a.x_coord, this->y_coord + vec_a.y_coord, new_color); 
}

Math_vector Math_vector::operator = (const Math_vector &vec_a)
{
    this->x_coord = vec_a.x_coord;
    this->y_coord = vec_a.y_coord;
    this->color_ = vec_a.color_;
    return *this; 
}

Math_vector Math_vector::operator - (const Math_vector &vec_a)
{
    sf::Color new_color ((this->color_.r + vec_a.color_.r) / 2, (this->color_.g + vec_a.color_.g) / 2,
                        (this->color_.b + vec_a.color_.b) / 2,(this->color_.a + vec_a.color_.a) / 2);
    return Math_vector (this->x_coord - vec_a.x_coord, this->y_coord - vec_a.y_coord, new_color); 
}

Math_vector Math_vector::operator - ()
{
    return Math_vector (-this->x_coord, -this->y_coord, this->color_); 
}

void Math_vector::draw (const Coord_system &coord_system, sf::RenderTarget &target, const Point start)
{
    int len = sqrt (this->y_coord*this->y_coord + this->x_coord*this->x_coord);

    Math_vector normal   (-(this->y_coord) / (len >> 1), this->x_coord / (len >> 1), this->color_);
    Math_vector opposite (  this->x_coord / (len >> 2), this->y_coord / (len >> 2), this->color_);
    Math_vector arrow_l = normal - opposite;
    Math_vector arrow_r = -normal - opposite;

    Point arrow_start(start.x_ + this->x_coord, start.y_ + this->y_coord);

    draw_line (*this, coord_system, target, start);
    draw_line (arrow_l, coord_system, target, arrow_start);
    draw_line (arrow_r, coord_system, target, arrow_start);
    
    normal.~Math_vector ();
    opposite.~Math_vector ();
    arrow_l.~Math_vector ();
    arrow_r.~Math_vector ();
    arrow_start.~Point ();
}

void draw_line (const Math_vector &vec, const Coord_system &coord_system, sf::RenderTarget &target, const Point start)
{
    sf::VertexArray lines(sf::Lines, 2);

    Point real_start (start.x_ + coord_system.x_start_, -start.y_ + coord_system.y_start_);

    lines[0].position = sf::Vector2f(real_start.x_, real_start.y_);
    lines[1].position = sf::Vector2f(real_start.x_ + vec.x_coord, real_start.y_ - vec.y_coord);
    lines[0].color = lines[1].color = vec.color_;

    target.draw (lines);
}