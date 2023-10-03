#include "vector.h"

void Vector::rotate (double deg)
{  
    double radians = deg * PI / 180.0;
    
    double deg_cos = cos (radians);
    double deg_sin = sin (radians);
    
    double x = x_;
    double y = y_;

    x_ = deg_cos * x - deg_sin * y; ///(cos_a + sin_a * i) * (x + y * i) = (cos_a * x - sin_a * y) + (y * cos_a + sin_a * x)
    y_ = deg_cos * y + deg_sin * x;
}

Vector Vector::operator * (const Vector &vec) const
{
    return Vector (y_ * vec.z_ - z_ * vec.y_, x_ * vec.z_ - z_ * vec.x_, x_ * vec.y_ - y_ * vec.x_);
}

Vector Vector::operator += (const Vector &vec)
{
    x_ += vec.x_;
    y_ += vec.y_;
    z_ += vec.z_;
    return *this;
}

Vector Vector::operator -= (const Vector &vec)
{
    x_ -= vec.x_;
    y_ -= vec.y_;
    z_ -= vec.z_;
    return *this;
}

Vector Vector::operator + (const Vector &vec) const
{
    return Vector (x_ + vec.x_, y_ + vec.y_, z_ + vec.z_);
}

Vector Vector::operator - (const Vector &vec) const
{
    return Vector (x_ - vec.x_, y_ - vec.y_, z_ - vec.z_);
}

Vector Vector::operator ! () const
{
    double module = sqrt (x_ * x_ + y_ * y_  + z_ * z_);

    return Vector (x_ / module, y_ / module, z_ / module);
}

Vector Vector::operator - () const
{
    return Vector (-x_, -y_, -z_);
}

Vector Vector::operator && (const double mult) const
{
    return Vector (x_ * mult, y_ * mult, z_ * mult);
}

double Vector::operator || (const Vector &vec) const 
{
    return x_ * vec.x_ + y_  * vec.y_ + z_ * vec.z_;
}

Point Vector::vec_to_point () 
{
    return Point (x_, y_, z_);
}

// // may be made base class for text and image to add it as parameter in button and use int it whether image or text
// //image of our size-->texture-->sprite-->sprite.set_texture () and sprite.set_texture_rect ()
