#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>

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

#include "../../graphics/curve_plot.h"
#include "../filter/filter.h"
#include "../plugin_data.h"

static const double DEFAULT_CIRCLE_THICKNESS    = 2.0;
static const double    MIN_CIRCLE_THICKNESS     = 1.0;
static const double DEFAULT_BRUSH_THICKNESS     = 2.0;

enum class BrushKeyPointsNum
{
    Zero,
    One,
    Two,
    ThreeOrMore
};

class Filter;
class ColorPalette;

class Tool : public plug::Tool
{    
protected:
    plug::Widget *widget_ = nullptr;
    plug::ControlState state_;
    plug::ColorPalette *color_palette_ = nullptr;
    plug::Canvas *active_canvas_ = nullptr;
    size_t ref_num_ = 0;
    MDNPluginData plugin_data_;

public:
    Tool (const char *name, const char *texture_path) : plugin_data_ (name, texture_path) {};
    Tool () = default;
    virtual ~Tool () = default;

    void setColorPalette (plug::ColorPalette &palette)  override 
        {color_palette_ = &palette;};
    void setActiveCanvas (plug::Canvas &canvas)         override 
        {active_canvas_ = (plug::Canvas *)&canvas;};

    virtual void onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos) override = 0;
    virtual void onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos) override = 0;
    virtual void onModifier1          (const plug::ControlState &control_state) override = 0;
    virtual void onModifier2          (const plug::ControlState &control_state) override = 0;
    virtual void onModifier3          (const plug::ControlState &control_state) override = 0;

    virtual void onMove                (const plug::Vec2d &pos) override = 0;
    virtual void onConfirm             () override = 0;
    virtual void onCancel              () override = 0;

    plug::Widget *getWidget () override {return widget_;};

    virtual plug::Plugin *tryGetInterface (size_t type)     override 
        {return ((plug::PluginGuid)type == plug::PluginGuid::Tool) ? this : nullptr;};	
    virtual void addReference ()                            override 
        {++ref_num_;};
    virtual void release ()                                 override 
        {if (!(--ref_num_)) delete this;};
    virtual const plug::PluginData *getPluginData () const  override 
        {return &plugin_data_;};
};

class Brush : public Tool
{
    bool is_used_ = false;
    
    plug::Vec2d last_point[3] = {};
    enum BrushKeyPointsNum key_points_num_ = BrushKeyPointsNum::Zero;

public:
    Brush ();
    ~Brush ();

    void onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos) override;
    void onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos) override;
    void onModifier1          (const plug::ControlState &control_state) override;
    void onModifier2          (const plug::ControlState &control_state) override;
    void onModifier3          (const plug::ControlState &control_state) override;

    void onMove                (const plug::Vec2d &pos) override;
    void onConfirm             () override;
    void onCancel              () override;
    // void on_released_key        () override {};
};

class Line : public Tool 
{
    plug::VertexArray vertex;
    plug::Vec2d latest_pos_ = plug::Vec2d ();

public:
    Line ();
    ~Line ();

    void onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos) override;
    void onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos) override;
    void onModifier1          (const plug::ControlState &control_state) override;
    void onModifier2          (const plug::ControlState &control_state) override;
    void onModifier3          (const plug::ControlState &control_state) override;

    void onMove                (const plug::Vec2d &pos) override;
    void onConfirm             () override;
    void onCancel              () override;
    // void on_released_key        () override {};
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

    void onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos) override;
    void onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos) override;
    void onModifier1          (const plug::ControlState &control_state) override;
    void onModifier2          (const plug::ControlState &control_state) override;
    void onModifier3          (const plug::ControlState &control_state) override;

    void onMove                (const plug::Vec2d &pos) override;
    void onConfirm             () override;
    void onCancel              () override;
    // void on_released_key        () override {is_on_modifier_1_ = false;};
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

    void onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos) override;
    void onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos) override;
    void onModifier1          (const plug::ControlState &control_state) override;
    void onModifier2          (const plug::ControlState &control_state) override;
    void onModifier3          (const plug::ControlState &control_state) override;

    void onMove                (const plug::Vec2d &pos) override;
    void onConfirm             () override;
    void onCancel              () override;
    // void on_released_key        () override {is_on_modifier_1_ = false;};

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

    void onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos) override;
    void onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos) override;
    void onModifier1          (const plug::ControlState &control_state) override;
    void onModifier2          (const plug::ControlState &control_state) override;
    void onModifier3          (const plug::ControlState &control_state) override;

    void onMove                (const plug::Vec2d &pos) override;
    void onConfirm             () override;
    void onCancel              () override;
    // void on_released_key        () override {};

private:
    void fill_pixels (plug::Vec2d pos);
};


class Filter_tool : public Tool 
{
    plug::Filter *filter_ = nullptr;

public:
    Filter_tool (plug::Filter *filter) : filter_(filter) {};
    ~Filter_tool () = default;
    
    void onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos) override;
    void onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos) override;
    void onModifier1          (const plug::ControlState &control_state) override;
    void onModifier2          (const plug::ControlState &control_state) override;
    void onModifier3          (const plug::ControlState &control_state) override;

    void onMove                (const plug::Vec2d &pos) override;
    void onConfirm             () override;
    void onCancel              () override;
    // void on_released_key        () override {};
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

    void onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos) override;
    void onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos) override;
    void onModifier1          (const plug::ControlState &control_state) override;
    void onModifier2          (const plug::ControlState &control_state) override;
    void onModifier3          (const plug::ControlState &control_state) override;

    void onMove                (const plug::Vec2d &pos) override;
    void onConfirm             () override;
    void onCancel              () override;
    plug::Widget *getWidget    () override 
        {if (rect_tool.getWidget ()) return rect_tool.getWidget ();   return widget_;};

    // void on_released_key        () override {if (on_rect_) rect_tool.on_released_key ();};
};


class Curve_filter;
class CurveTool : public Tool 
{
    CurvePlot plot;
    Curve_filter *filter = nullptr;
    plug::Vec2d prev_pos_ = plug::Vec2d (-1, -1);
    bool is_called_before = false;
    int last_point_idx = -1;
    plug::VertexArray vertices_ = plug::VertexArray (plug::Quads, 4);
public:
    CurveTool ();
    ~CurveTool ();

    void onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos) override;
    void onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos) override;
    void onModifier1          (const plug::ControlState &control_state) override;
    void onModifier2          (const plug::ControlState &control_state) override;
    void onModifier3          (const plug::ControlState &control_state) override;

    void onMove                (const plug::Vec2d &pos) override;
    void onConfirm             () override;
    void onCancel              () override;
private:
    void init_vertices (plug::Texture &texture);
};

#endif /* TOOLS_H */