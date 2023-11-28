#ifndef PLUGIN_DATA_H
#define PLUGIN_DATA_H

#include <iostream>
#include "../standard/plug_plugin.h"

class PluginData : public plug::PluginData
{
    char *name_ = nullptr;
    char *path_ = nullptr;

public:
    PluginData () = default;
    PluginData (const char *name, const char *texture_path);
    
    const char *getName () const override {return name_;};
    const char *getTexturePath () const override {return path_;};

    ~PluginData ();
};

#endif /* PLUGIN_DATA_H */