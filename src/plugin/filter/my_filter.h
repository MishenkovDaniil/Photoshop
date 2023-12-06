#ifndef MY_FILTER_H
#define MY_FILTER_H

#include "../../standard/standard.h"
#include "../../standard/plug_plugin.h"
#include "../plugin_data.h"
#include "../../graphic_structures/color/hsl_luma_color.h"

static const double MINIMUM_SATURATION_VAL = 0.05;

class MPluginData : public plug::PluginData
{
    char *name_ = nullptr;
    char *path_ = nullptr;

public:
    MPluginData () = default;
    MPluginData (const char *name, const char *texture_path);
    
    const char *getName () const override {return name_;};
    const char *getTexturePath () const override {return path_;};

    ~MPluginData ();
};

class MYFilter : public plug::Filter
{
protected:
    plug::Widget *filter_widget = nullptr;
    size_t ref_num_ = 0;
    MPluginData plugin_data_;

public:
    MYFilter (const char *name, const char *texture_path = nullptr) : plugin_data_ (name, texture_path) {};
    MYFilter () = default;
    ~MYFilter () = default;

    virtual void applyFilter (plug::Canvas &canvas) const override = 0;
    plug::Widget *getWidget () {return filter_widget;};

    virtual plug::Plugin *tryGetInterface (size_t type)     override 
        {return ((plug::PluginGuid)type == plug::PluginGuid::Filter) ? this : nullptr;};	
    virtual void addReference ()                            override 
        {++ref_num_;};
    virtual void release ()                                 override 
        {if (!(--ref_num_)) delete this;};
    virtual const plug::PluginData *getPluginData () const {return &plugin_data_;};
};

class Saturation_filter : public MYFilter
{
    double delta_saturation_ = 0; ///in percent
public:
    Saturation_filter (double delta_saturation = 10) :   MYFilter (((delta_saturation > 0) ? "satur++" : "satur--")), 
                                                        delta_saturation_ (delta_saturation) {};
    ~Saturation_filter () = default;

    void applyFilter (plug::Canvas &canvas) const override;
};


#endif /* MY_FILTER_H */