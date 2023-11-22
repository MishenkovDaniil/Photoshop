#ifndef PLUG_PLUGIN_H
#define PLUG_PLUGIN_H

#include <iostream>

namespace plug
{
    class PluginData 
    {
        virtual const char *getName () const = 0;
        virtual const char *getTexturePath () const = 0;

        virtual ~PluginData () = 0;
    };

    class Plugin 
    {
        virtual Plugin *tryGetInterface (size_t) = 0;	// аналог  QueryInterface
        virtual void addReference (Plugin *) = 0;
        virtual void release (Plugin *) = 0;
        virtual const PluginData *getPluginData () const = 0;
    };
    
    enum class PluginGuid
    {
        Plugin 	= 0,
        Tool 	= 1,
        Filter 	= 2
    };
}

#endif /* PLUG_PLUGIN_H */