#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "../../widget/widget.h"
#include "../../widget/window/canvas/canvas.h"
#include "../../widget/window/window.h"
#include "../filter/filter.h"
#include "../../widget/text/text.h"

#include "../../standard/plug_tool.h"
#include "../../standard/plug_plugin.h"
#include "../../standard/plug_filter.h"
#include "../../graphics/intrect/intrect.h"
#include "../../graphics/circleshape/circleshape.h"
#include "../../graphics/rectangleshape/rectangleshape.h"

#include "../plugin_data.h"
static const double DEFAULT_CIRCLE_THICKNESS    = 2.0;
static const double    MIN_CIRCLE_THICKNESS     = 1.0;
static const double DEFAULT_BRUSH_THICKNESS     = 10.0;

class Filter;
class ColorPalette;

class Tool : public plug::Tool
{    
protected:
    Widget *widget_ = nullptr;
    plug::ControlState state_;
    plug::ColorPalette *color_palette_ = nullptr;
    plug::Canvas *active_canvas_ = nullptr;
    size_t ref_num_ = 0;
    PluginData plugin_data_;

public:
    Tool (const char *name, const char *texture_path) : plugin_data_ (name, texture_path) {};
    Tool () = default;
    virtual ~Tool () = default;

    void setColorPalette (plug::ColorPalette &palette)  override 
        {color_palette_ = &palette;};
    void setActiveCanvas (plug::Canvas &canvas)         override 
        {active_canvas_ = (plug::Canvas *)&canvas;};

    virtual void on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos) = 0;
    virtual void on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos) = 0;
    virtual void on_modifier_1          (const plug::ControlState &control_state) = 0;
    virtual void on_modifier_2          (const plug::ControlState &control_state) = 0;
    virtual void on_modifier_3          (const plug::ControlState &control_state) = 0;

    virtual void on_move                (plug::Vec2d &pos) = 0;
    virtual void on_confirm             () = 0;
    virtual void on_cancel              () = 0;
    virtual void on_released_key        () = 0;

    Widget *get_widget () {return widget_;};

    virtual plug::Plugin *tryGetInterface (size_t type)     override 
        {return ((plug::PluginGuid)type == plug::PluginGuid::Tool) ? this : nullptr;};	
    virtual void addReference (plug::Plugin *)              override 
        {++ref_num_;};
    virtual void release (plug::Plugin *)                   override 
        {if (!(--ref_num_)) delete this;};
    virtual const plug::PluginData *getPluginData () const  override 
        {return &plugin_data_;};
};

class Brush : public Tool
{
    plug::Vec2d prev_pos = 0;
    bool is_pressed = false;

public:
    Brush ();
    ~Brush ();

    void on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos) override;
    void on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos) override;
    void on_modifier_1          (const plug::ControlState &control_state) override;
    void on_modifier_2          (const plug::ControlState &control_state) override;
    void on_modifier_3          (const plug::ControlState &control_state) override;

    void on_move                (plug::Vec2d &pos) override;
    void on_confirm             () override;
    void on_cancel              () override;
    void on_released_key        () override {};
};

class Line : public Tool 
{
    plug::VertexArray vertex;
    plug::Vec2d latest_pos_ = plug::Vec2d ();

public:
    Line ();
    ~Line ();

    void on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos) override;
    void on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos) override;
    void on_modifier_1          (const plug::ControlState &control_state) override;
    void on_modifier_2          (const plug::ControlState &control_state) override;
    void on_modifier_3          (const plug::ControlState &control_state) override;

    void on_move                (plug::Vec2d &pos) override;
    void on_confirm             () override;
    void on_cancel              () override;
    void on_released_key        () override {};
};

// class Polygon : public Tool

class Circle_shape : public Tool
{
    plug::Vec2d center_;
    plug::Vec2d last_center_;
    CircleShape circle_;
    plug::Vec2d latest_pos_ = plug::Vec2d ();
    bool is_on_modifier_1_ = false;

public:
    Circle_shape ();
    ~Circle_shape ();

    void on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos) override;
    void on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos) override;
    void on_modifier_1          (const plug::ControlState &control_state) override;
    void on_modifier_2          (const plug::ControlState &control_state) override;
    void on_modifier_3          (const plug::ControlState &control_state) override;

    void on_move                (plug::Vec2d &pos) override;
    void on_confirm             () override;
    void on_cancel              () override;
    void on_released_key        () override {is_on_modifier_1_ = false;};
};

class Rect_shape : public Tool 
{
    plug::Vec2d center_;
    plug::Vec2d last_center_;
    RectangleShape rect_;
    plug::Color canvas_bg_color = plug::Transparent;
    plug::Vec2d latest_pos_ = plug::Vec2d ();
    bool is_on_modifier_1_ = false;

public:
    Rect_shape ();
    ~Rect_shape ();

    void on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos) override;
    void on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos) override;
    void on_modifier_1          (const plug::ControlState &control_state) override;
    void on_modifier_2          (const plug::ControlState &control_state) override;
    void on_modifier_3          (const plug::ControlState &control_state) override;

    void on_move                (plug::Vec2d &pos) override;
    void on_confirm             () override;
    void on_cancel              () override;
    void on_released_key        () override {is_on_modifier_1_ = false;};

    friend class Text_tool;
};

class Fill : public Tool
{
    plug::Color *pixel_arr_ = nullptr;
    plug::Vec2d size_;

    plug::Color fill_color_;
    plug::Color cur_color_;

    Sprite draw_sprite_;

public:
    Fill ();
    ~Fill ();

    void on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos) override;
    void on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos) override;
    void on_modifier_1          (const plug::ControlState &control_state) override;
    void on_modifier_2          (const plug::ControlState &control_state) override;
    void on_modifier_3          (const plug::ControlState &control_state) override;

    void on_move                (plug::Vec2d &pos) override;
    void on_confirm             () override;
    void on_cancel              () override;
    void on_released_key        () override {};

private:
    void fill_pixels (plug::Vec2d &pos);
};


class Filter_tool : public Tool 
{
    plug::Filter *filter_ = nullptr;

public:
    Filter_tool (plug::Filter *filter) : filter_(filter) {};
    ~Filter_tool () = default;
    
    void on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos) override;
    void on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos) override;
    void on_modifier_1          (const plug::ControlState &control_state) override;
    void on_modifier_2          (const plug::ControlState &control_state) override;
    void on_modifier_3          (const plug::ControlState &control_state) override;

    void on_move                (plug::Vec2d &pos) override;
    void on_confirm             () override;
    void on_cancel              () override;
    void on_released_key        () override {};
};

class Text_tool : public Tool
{
    const char buf[512] = {};
    Rect_shape rect_tool = Rect_shape ();
    bool on_rect_ = false;
    plug::Vec2d latest_pos_ = plug::Vec2d (0, 0);

public:
    Text_tool ();
    ~Text_tool ();

    void setColorPalette (plug::ColorPalette &palette) override {color_palette_ = &palette; rect_tool.color_palette_ = &palette;};
    void on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos) override;
    void on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos) override;
    void on_modifier_1          (const plug::ControlState &control_state) override;
    void on_modifier_2          (const plug::ControlState &control_state) override;
    void on_modifier_3          (const plug::ControlState &control_state) override;

    void on_move                (plug::Vec2d &pos) override;
    void on_confirm             () override;
    void on_cancel              () override;
    void on_released_key        () override {if (on_rect_) rect_tool.on_released_key ();};
};

#endif /* TOOLS_H */