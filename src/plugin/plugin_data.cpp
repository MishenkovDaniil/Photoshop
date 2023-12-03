#include <cassert>
#include <cstring>

#include "plugin_data.h"

MishenkovPluginData::MishenkovPluginData (const char *name, const char *texture_path)
{
    if (name)
    {
        name_ = new char[strlen (name) + 1];
        assert (name_ && "Error: failed to allocate memory for plugin name.\n");
        strcpy (name_, name);
    } 
    if (texture_path) 
    {
        path_ = new char[strlen (texture_path) + 1];
        assert (path_ && "Error: failed to allocate memory for plugin path.\n");
        strcpy (path_, texture_path);
    }
}

MishenkovPluginData::~MishenkovPluginData ()
{
    if (name_)
    {
        delete[] name_;
        name_ = nullptr;
    }
    if (path_)
    {
        delete[] path_;
        path_ = nullptr;
    }
}