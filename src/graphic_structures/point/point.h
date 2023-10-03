#ifndef POINT_H
#define POINT_H

class Point 
{
public:

    double x_ = 0;
    double y_ = 0;
    double z_ = 0;

    
public:
    Point (double x = 0, double y = 0, double z = 0) : x_ (x), y_ (y), z_ (z) {};
    Point (const Point &point) : x_ (point.x_), y_ (point.y_), z_ (point.z_) {};
    ~Point ();
    
    void update (double x, double y, double z);
    double distance (Point &point);
};

#endif /* POINT_H */