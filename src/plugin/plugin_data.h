#ifndef PLUGIN_DATA_H
#define PLUGIN_DATA_H

#include <iostream>
#include "../standard/plug_plugin.h"

class MDNPluginData : public plug::PluginData
{
    char *name_ = nullptr;
    char *path_ = nullptr;

public:
    MDNPluginData () = default;
    MDNPluginData (const char *name, const char *texture_path);
    
    const char *getName () const override {return name_;};
    const char *getTexturePath () const override {return path_;};

    ~MDNPluginData ();
};

#endif /* PLUGIN_DATA_H */