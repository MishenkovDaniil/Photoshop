#ifndef PLUGIN_H
#define PLUGIN_H

#include <iostream>

namespace plug
{
    class Plugin 
    {
        virtual Plugin *tryGetInterface (size_t) = 0;	// аналог  QueryInterface
        virtual void addReference (Plugin *) = 0;
        virtual void release (Plugin *) = 0;
        virtual const PluginData *getPluginData () const = 0;
    };

    class PluginData 
    {
        const char *getName () const;
        const char *getTexturePath () const;
    };
        
    enum class PluginGuid
    {
        Plugin 	= 0,
        Tool 	= 1,
        Filter 	= 2
    };
}

#endif /* PLUGIN_H */