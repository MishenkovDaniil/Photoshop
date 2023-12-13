#ifndef FILTER_H
#define FILTER_H

#include "../../graphics/curve_plot/curve_plot.h"
#include "../../widget/widget.h"
#include "../../widget/window/canvas/canvas.h"
#include "../../widget/window/window.h"
#include "../../standard/plug_filter.h"
#include "../../standard/plug_canvas.h"
#include "../plugin_data.h"

static const double MIN_SATURATION_VAL = 0.05;
static const double MIN_LIGHT_VAL = 0.05;

class Filter : public plug::Filter
{
    plug::Widget *filter_widget = nullptr;
    size_t ref_num_ = 0;
    MDNPluginData plugin_data_;

public:
    Filter (const char *name, const char *texture_path = nullptr) : plugin_data_ (name, texture_path) {};
    Filter () = default;
    ~Filter () = default;

    virtual void applyFilter (plug::Canvas &canvas) const override = 0;
    plug::Widget *getWidget () {return filter_widget;};

    virtual plug::Plugin *tryGetInterface (size_t type)     override 
        {return ((plug::PluginGuid)type == plug::PluginGuid::Filter) ? this : nullptr;};	
    virtual void addReference ()                            override 
        {++ref_num_;};
    virtual void release ()                                 override 
        {if (!(--ref_num_)) delete this;};
    virtual const plug::PluginData *getPluginData () const  override 
        {return &plugin_data_;};

};

class Light_filter : public Filter
{
    double delta_light_ = 0;   ///in percent
public:
    Light_filter (double delta_light) : Filter (((delta_light > 0) ? "light++" : "light--")),
                                        delta_light_ (delta_light) {};
    ~Light_filter () = default;

    void applyFilter (plug::Canvas &canvas) const override;
};

// class Saturation_filter : public Filter
// {
//     double delta_saturation_ = 0; ///in percent
// public:
//     Saturation_filter (double delta_saturation = 10) :   Filter (((delta_saturation > 0) ? "satur++" : "satur--")), 
//                                                         delta_saturation_ (delta_saturation) {};
//     ~Saturation_filter () = default;

//     void applyFilter (plug::Canvas &canvas) const override;
// };


class Curve_filter : public Filter
{
    Button *curve_manage_button_ = nullptr;
public:
    Curve_filter (Button *curve_manage_button) : Filter ( "curves"), curve_manage_button_ (curve_manage_button) {};
    ~Curve_filter () = default;

    void applyFilter (plug::Canvas &canvas) const override;
    void applyCurveFilter (plug::Canvas &canvas, CurvePlot &plot) const;
};

class White_black_filter : public Filter
{
public:
    White_black_filter () : Filter ("white&black") {};
    ~White_black_filter () = default;

    void applyFilter (plug::Canvas &canvas) const override;
};

class Filter_palette 
{
    M_vector<plug::Filter *>filters = M_vector<plug::Filter *> (nullptr);
    plug::Filter *last_filter_ = nullptr;

public:
    plug::Filter *get_last_filter ();
    void    set_last_filter (plug::Filter *filter);
    plug::Filter *get_filter      (size_t filter_idx);
    void    add_filter (plug::Filter *filter); 
    size_t get_filter_count ();    
};

#endif /* FILTER_H */