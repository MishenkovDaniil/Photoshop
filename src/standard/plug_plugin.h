#ifndef PLUG_PLUGIN_H
#define PLUG_PLUGIN_H

#include <iostream>

namespace plug
{
    class PluginData 
    {
    public:
        virtual const char *getName () const = 0;
        virtual const char *getTexturePath () const = 0;

        virtual ~PluginData () = default;
    };

    class Plugin 
    {
    public:
        virtual Plugin *tryGetInterface (size_t) = 0;	/// аналог  QueryInterface
        virtual void addReference (Plugin *) = 0;
        virtual void release (Plugin *) = 0;
        virtual const PluginData *getPluginData () const = 0;
        
        virtual ~Plugin () = default;
    };
    
    enum class PluginGuid
    {
        Plugin 	= 0,
        Tool 	= 1,
        Filter 	= 2
    };
}

#endif /* PLUG_PLUGIN_H */