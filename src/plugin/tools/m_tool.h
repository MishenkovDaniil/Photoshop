#ifndef M_TOOL_H
#define M_TOOL_H

#include <iostream>

#include "../../standard/plug_tool.h"
#include "../../standard/plug_plugin.h"
#include "../../standard/plug_widget.h"
#include "../../standard/plug_canvas.h"

#include "../plugin_data.h"

static const double DEFAULT_CIRCLE_THICKNESS    = 2.0;
static const double    MIN_CIRCLE_THICKNESS     = 1.0;
static const double DEFAULT_BRUSH_THICKNESS     = 10.0;

class MTool : public plug::Tool
{    
protected:
    plug::Widget *widget_ = nullptr;
    plug::ControlState state_;
    plug::ColorPalette *color_palette_ = nullptr;
    plug::Canvas *active_canvas_ = nullptr;
    size_t ref_num_ = 0;
    MDNPluginData plugin_data_;

public:
    MTool (const char *name, const char *texture_path) : plugin_data_ (name, texture_path) {};
    MTool () = default;
    virtual ~MTool () = default;

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

class MRectangle
{
    plug::VertexArray rect_ = plug::VertexArray (plug::Lines, 8);
    plug::Vec2d position_;
    plug::Vec2d size_;
    plug::Color color_;

public:
    MRectangle () = default;
    ~MRectangle () = default;

    void setPosition (const plug::Vec2d &position) {position_ = position;};
    void setColor (plug::Color color) {color_ = color;};
    void setSize (plug::Vec2d size) {size_ = size;};
    plug::VertexArray getRect ();
};

class Rect_shape : public MTool 
{
    plug::Vec2d center_;
    plug::Vec2d last_center_;
    MRectangle rect_;
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
};


#endif /* M_TOOL_H */