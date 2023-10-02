#ifndef PLOT_H
#define PLOT_H

#include <cassert>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdlib.h>

#include "../point/point.h"
#include "../color/color.h"
#include "../coord_system/coord_system.h"

class Plot 
{
    int size_ = 0;
    double abscissa_len_ = 0;
    double ordinate_len_ = 0;
    double max_val_ = 0;

    Point *points_ = nullptr;
    Point &start_;
    
    int cur = 0;
    bool is_filled = false;
    
    Coord_system coord_system;
    Color color_;
    sf::VertexArray *array;

public:
    Plot (Point &start, int x_len, int y_len, double max_val, Color color = Color (255, 0, 0, 255));
    ~Plot ();
    void draw (sf::RenderTarget &target, double param);
};


#endif /* PLOT_H */