#include <cmath>

#include "point.h"


void Point::update (double x, double y, double z)
{
    x_ = x;
    y_ = y;
    z_ = z;
}

double Point::distance (Point &point)
{
    return sqrt((x_ - point.x_) * (x_ - point.x_) + (y_ - point.y_) * (y_ - point.y_) + (z_ - point.z_) * (z_ - point.z_));
}

Point::~Point ()
{
    this->x_ = NAN;
    this->y_ = NAN;  
    this->z_ = NAN;  
}
