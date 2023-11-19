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

class Filter_mask
{
    bool *mask = nullptr;
    size_t width_  = 0;
    size_t height_ = 0;

public:
    Filter_mask (size_t width, size_t height) : width_ (width), height_ (height) 
                                                { mask = new bool[width * height];
                                                  assert (mask);};
    ~Filter_mask () {delete[] mask;};
    size_t get_width () {return width_;};
    size_t get_height () {return height_;};

    bool get_pixel (size_t x, size_t y);  
    void set_pixel (size_t x, size_t y, bool flag);  
    void fill (bool value);
    //TODO :: void invert ();
};

class Filter
{
public:
    virtual void apply_filter (Canvas &canvas, Filter_mask *mask) const = 0;
};

class Light_filter : public Filter
{
    double delta_light_ = 0;   ///in percent
public:
    Light_filter (double delta_light) : delta_light_ (delta_light) {};
    ~Light_filter () = default;

    void apply_filter (Canvas &canvas, Filter_mask *mask) const override;
};

class Saturation_filter : public Filter
{
    double delta_saturation_ = 0; ///in percent
public:
    Saturation_filter (double delta_saturation) : delta_saturation_ (delta_saturation) {};
    ~Saturation_filter () = default;

    void apply_filter (Canvas &canvas, Filter_mask *mask) const override;
};

class White_black_filter : public Filter
{
public:
    White_black_filter () {};
    ~White_black_filter () = default;

    void apply_filter (Canvas &canvas, Filter_mask *mask) const override;
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