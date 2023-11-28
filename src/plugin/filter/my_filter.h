#ifndef MY_FILTER_H
#define MY_FILTER_H

#include "../../standard/standard.h"
#include "../../widget/window/canvas/canvas.h"
#include "../../standard/plug_plugin.h"
#include "../plugin_data.h"


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

    virtual void apply_filter (plug::Canvas &canvas) const override = 0;
    plug::Widget *getWidget () {return filter_widget;};

    virtual plug::Plugin *tryGetInterface (size_t type)     override 
        {return ((plug::PluginGuid)type == plug::PluginGuid::Filter) ? this : nullptr;};	
    virtual void addReference (plug::Plugin *)              override 
        {++ref_num_;};
    virtual void release (plug::Plugin *)                   override 
        {if (!(--ref_num_)) delete this;};
    virtual const plug::PluginData *getPluginData () const {return &plugin_data_;};
};

class White_black_filter : public MYFilter
{
public:
    White_black_filter () : MYFilter ("White&black") {};
    // White_black_filter () = default;
    ~White_black_filter () = default;

    void apply_filter (plug::Canvas &canvas) const override;
};

#endif /* MY_FILTER_H */