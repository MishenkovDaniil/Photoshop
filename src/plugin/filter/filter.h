#ifndef FILTER_H
#define FILTER_H

#include "../../widget/widget.h"
#include "../../widget/window/canvas/canvas.h"

static const double MIN_SATURATION_VAL = 0.05;
static const double MIN_LIGHT_VAL = 0.05;

enum Filters 
{
    Unknown_filter,
    Light_incr,
    Light_decr    
};

class Filter
{
    Widget *filter_widget = nullptr;
public:
    Filter () = default;
    ~Filter () = default;

    virtual void apply_filter (Canvas &canvas) const = 0;
    Widget *getWidget () {return filter_widget;};
};

class Light_filter : public Filter
{
    double delta_light_ = 0;   ///in percent
public:
    Light_filter (double delta_light) : delta_light_ (delta_light) {};
    ~Light_filter () = default;

    void apply_filter (Canvas &canvas) const override;
};

class Saturation_filter : public Filter
{
    double delta_saturation_ = 0; ///in percent
public:
    Saturation_filter (double delta_saturation) : delta_saturation_ (delta_saturation) {};
    ~Saturation_filter () = default;

    void apply_filter (Canvas &canvas) const override;
};

class White_black_filter : public Filter
{
public:
    White_black_filter () {};
    ~White_black_filter () = default;

    void apply_filter (Canvas &canvas) const override;
};

class Filter_palette 
{
    M_vector<Filter *>filters = M_vector<Filter *> (nullptr);
    Filter *last_filter_ = nullptr;

public:
    Filter *get_last_filter ();
    void    set_last_filter (Filter *filter);
    Filter *get_filter      (size_t filter_idx);
    void    add_filter (Filter *filter); 
    size_t get_filter_count ();    
};

#endif /* FILTER_H */