#include "coord_system.h"
#include "../math_vector/math_vector.h"
#include "assert.h"
#include "../point/point.h"

Coord_system::Coord_system (int x_start, int y_start, int x_len, int y_len, int x_shift, int y_shift, sf::VideoMode videomode) :
    x_start_ (x_start),
    y_start_ (y_start),
    x_shift_ (x_shift),
    y_shift_ (y_shift),
    x_len_ (x_len),
    y_len_ (y_len),
    window_len_x (videomode.width),
    window_len_y (videomode.height)
    {
        abscissa  = new Math_vector (x_len_, 0, sf::Color::White);
        assert (abscissa);
        ordinate  = new Math_vector (0, y_len, sf::Color::White);
        assert (ordinate);
        // fprintf (stderr, "[%p]\n[%p], ")
    };
 
Coord_system:: ~Coord_system ()
{
    delete abscissa;
    delete ordinate;
};

void Coord_system::start (Point &point) { point.x_ = x_start_; point.y_ = y_start_;};


void Coord_system::draw (sf::RenderTarget &target)
{
    assert (abscissa && ordinate);

    if (window_len_x - x_start_ >= x_len_ / 2 && x_start_ >= x_len_ / 2)
    {
        this->abscissa->draw (*this, target, Point (- x_len_ / 2, 0));
    }
    else if (window_len_x - x_start_ < x_len_ / 2 )
    {
        this->abscissa->draw (*this, target, Point (-(x_len_ - (window_len_x - x_start_)), 0));
    }
    else 
    {
        this->abscissa->draw (*this, target, Point (-x_start_, 0));
    }

    if (window_len_y - y_start_ >= y_len_ / 2 && y_start_ >= y_len_ / 2)
    {
        this->ordinate->draw (*this, target, Point (0, - y_len_ / 2));
    }
    else if (y_start_ < y_len_ / 2)
    {
        this->ordinate->draw (*this, target, Point (0, - (y_len_ - y_start_)));
    }
    else
    {
        this->ordinate->draw (*this, target, Point (0, - (window_len_y - y_start_)));
    }
}

Coord_system Coord_system::operator = (const Coord_system &coord_system)
{
    y_shift_ = coord_system.y_shift_;
    x_shift_ = coord_system.x_shift_;
    y_len_   = coord_system.y_len_;
    x_len_   = coord_system.x_len_;
    window_len_x = coord_system.window_len_x;
    window_len_y = coord_system.window_len_y;
    x_start_ = coord_system.x_start_;
    y_start_ = coord_system.y_start_;
    
    return *this;
}
