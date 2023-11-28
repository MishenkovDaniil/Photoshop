#ifndef PLUG_TOOL_H
#define PLUG_TOOL_H

#include "../graphic_structures/vector/vector.h"
#include "plug_plugin.h"
#include "plug_widget.h"
#include "plug_colorpalette.h"
#include "plug_canvas.h"

namespace plug
{
    enum State
    {
        Pressed, 
        Released
    };

    struct ControlState 
    {
        State state = Released;
    };

    class Tool : public Plugin
    {    
    protected:
        Widget *widget_ = nullptr;
        ControlState state_;
        ColorPalette *color_palette_ = nullptr;
        Canvas *active_canvas_ = nullptr;

    public:
        virtual ~Tool () = default;

        virtual void setColorPalette (ColorPalette &palette) = 0;
        virtual void setActiveCanvas (Canvas &canvas) = 0; 	// храним канвас в самом туле чтобы не передавать его все время в качестве параметра и для того чтобы реагировать на изменение активного канваса.

        virtual void on_main_button         (const ControlState &control_state, Vec2d &pos) = 0;
        virtual void on_secondary_button    (const ControlState &control_state, Vec2d &pos) = 0;
        virtual void on_modifier_1          (const ControlState &control_state) = 0;
        virtual void on_modifier_2          (const ControlState &control_state) = 0;
        virtual void on_modifier_3          (const ControlState &control_state) = 0;

        virtual void on_move                (Vec2d &pos) = 0;
        virtual void on_confirm             () = 0;
        virtual void on_cancel              () = 0;
        virtual void on_released_key        () = 0;

        virtual Widget *get_widget () = 0;

        virtual Plugin *tryGetInterface (size_t) = 0;	
        virtual void addReference (Plugin *) = 0;
        virtual void release (Plugin *) = 0;
        virtual const PluginData *getPluginData () const = 0;
    };
}

#endif /* PLUG_TOOL_H */