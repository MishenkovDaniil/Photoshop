#include <iostream>
#include <cmath>

#include "../graphic_structures/vector/vector.h"
#include "../graphic_structures/color/color.h"
#include "../widget/texture_widget/texture_widget.h"

static const double CATMULL_ROM_DELTA = 0.01;

double calcT (const plug::Vec2d &point_1, const plug::Vec2d &point_2, double t, double alpha);
inline double iterateT (double start, double end, double delta);

void catmullRomCentralSplineDraw (const plug::Vec2d &point_1, const plug::Vec2d &point_2, 
                                  const plug::Vec2d &point_3,      const plug::Vec2d &point_4,
                                  double thickness, M_render_texture &widget, 
                                  const plug::Color draw_color, double alpha = 0.5);
void catmullRomRightSplineDraw   (const plug::Vec2d &point_1, const plug::Vec2d &point_2,
                                  const plug::Vec2d &point_3, double thickness, M_render_texture &widget, 
                                  const plug::Color draw_color, double alpha = 0.5);
void catmullRomLeftSplineDraw    (const plug::Vec2d &point_1, const plug::Vec2d &point_2, 
                                  const plug::Vec2d &point_3, double thickness, M_render_texture &widget, 
                                  const plug::Color draw_color, double alpha = 0.5);
void catmullRomLineDraw          (const plug::Vec2d &point_1, const plug::Vec2d &point_2,
                                  double thickness, M_render_texture &widget, 
                                  const plug::Color draw_color, double alpha = 0.5);