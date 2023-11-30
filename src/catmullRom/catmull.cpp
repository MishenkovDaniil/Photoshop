#include "catmull.h"

double calcT (const plug::Vec2d &point_1, const plug::Vec2d &point_2, double t, double alpha)
{
    plug::Vec2d sub = point_2 - point_1;
    double sub_dot = dot (sub, sub);
    double delta_t = std::pow (sub_dot, alpha * 0.5);
    return t + delta_t;
}

inline double iterateT (double start, double end, double delta)
{
    return start + delta * (end - start);
}

void catmullRomCentralSplineDraw   (const plug::Vec2d &point_1, const plug::Vec2d &point_2, 
                                    const plug::Vec2d &point_3, const plug::Vec2d &point_4,
                                    double thickness, M_render_texture &widget, 
                                    const plug::Color draw_color, double alpha)
{
    double t0 = 0;
    double t1 = calcT (point_1, point_2, t0, alpha);
    double t2 = calcT (point_2, point_3, t1, alpha);
    double t3 = calcT (point_3, point_4, t2, alpha);
    CircleShape circle;
    circle.setRadius (thickness);
    circle.setFillColor (draw_color);

    for (double i = 0; i <= 1; i += CATMULL_ROM_DELTA)
    {
        double t = iterateT (t1, t2, i);

        plug::Vec2d a1 = ((t1 - t) / (t1 - t0)) * point_1 + ((t - t0) / (t1 - t0)) * point_2;
        plug::Vec2d a2 = ((t2 - t) / (t2 - t1)) * point_2 + ((t - t1) / (t2 - t1)) * point_3;
        plug::Vec2d a3 = ((t3 - t) / (t3 - t2)) * point_3 + ((t - t2) / (t3 - t2)) * point_4;

        plug::Vec2d b1 = ((t2 - t) / (t2 - t0)) * a1 + ((t - t0) / (t2 - t0)) * a2;
        plug::Vec2d b2 = ((t3 - t) / (t3 - t1)) * a2 + ((t - t1) / (t3 - t1)) * a3;

        plug::Vec2d c = ((t2 - t) / (t2 - t1)) * b1 + ((t - t1) / (t2 - t1)) * b2;
        circle.setPosition (c);
        widget.draw (circle);
    }
}

void catmullRomRightSplineDraw (const plug::Vec2d &point_1, const plug::Vec2d &point_2,
                                const plug::Vec2d &point_3, double thickness, M_render_texture &widget, 
                                const plug::Color draw_color, double alpha)
{
    double t0 = 0;
    double t1 = calcT (point_1, point_2, t0, alpha);
    double t2 = calcT (point_2, point_3, t1, alpha);

    CircleShape circle;
    circle.setRadius (thickness);
    circle.setFillColor (draw_color);
    
    for (double i = 0; i <= 1; i += CATMULL_ROM_DELTA)
    {
        double t = iterateT (t1, t2, i);

        plug::Vec2d a1 = ((t1 - t) / (t1 - t0)) * point_1 + ((t - t0) / (t1 - t0)) * point_2;
        plug::Vec2d a2 = ((t2 - t) / (t2 - t1)) * point_2 + ((t - t1) / (t2 - t1)) * point_3;

        plug::Vec2d b1 = ((t2 - t) / (t2 - t0)) * a1 + ((t - t0) / (t2 - t0)) * a2;

        circle.setPosition (b1);
        widget.draw (circle);
    }
}

void catmullRomLeftSplineDraw  (const plug::Vec2d &point_1, const plug::Vec2d &point_2, 
                                const plug::Vec2d &point_3, double thickness, M_render_texture &widget, 
                                const plug::Color draw_color, double alpha)
{
    return catmullRomRightSplineDraw (point_3, point_2, point_1, thickness, widget, draw_color, alpha);
}

void catmullRomLineDraw (const plug::Vec2d &point_1, const plug::Vec2d &point_2,
                         double thickness, M_render_texture &widget, 
                         const plug::Color draw_color, double alpha)
{
    double t0 = 0;
    double t1 = calcT (point_1, point_2, t0, alpha);

    CircleShape circle;
    circle.setRadius (thickness);
    circle.setFillColor (draw_color);
    
    for (double i = 0; i <= 1; i += CATMULL_ROM_DELTA)
    {
        double t = iterateT (t0, t1, i);

        plug::Vec2d a1 = ((t1 - t) / (t1 - t0)) * point_1 + ((t - t0) / (t1 - t0)) * point_2;

        circle.setPosition (a1);
        widget.draw (circle);
    }
}