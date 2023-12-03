#ifndef PLUG_TOOL_H
#define PLUG_TOOL_H

#include "../graphic_structures/vector/vector.h"
#include "plug_plugin.h"
#include "plug_widget.h"
#include "plug_colorpalette.h"
#include "plug_canvas.h"


namespace plug {

/**
 * @brief Current state of tool control button
 */
enum class State {
  Pressed,  /*!< Button is pressed */
  Released, /*!< Button is released */
};

/**
 * @brief State of tool control
 */
struct ControlState {
  State state; /*!< State of tool control button */
};

} // namespace plug


namespace plug {

/**
 * @brief Interface for canvas tool plugin
 */
class Tool : public Plugin {
public:
  /**
   * @brief Set active color palette
   *
   * @param[in] palette	New active color palette
   */
  virtual void setColorPalette(ColorPalette &palette) = 0;

  /**
   * @brief Set active (currently edited) canvas
   *
   * @param[in] canvas	Currently edited canvas
   */
  virtual void setActiveCanvas(Canvas &) = 0;

  /**
   * @brief Process action on main tool button
   *
   * @param[in] state	    State of button
   * @param[in] position	Position of cursor during action
   */
  virtual void onMainButton(const ControlState &state, const Vec2d &) = 0;

  /**
   * @brief Process action on secondary tool button
   *
   * @param[in] state	    State of button
   * @param[in] position	Position of cursor during action
   */
  virtual void onSecondaryButton(const ControlState &state,
                                 const Vec2d &position) = 0;

  /**
   * @brief Process action on tool modifier button no. 1
   *
   * @param[in] state	    State of button
   */
  virtual void onModifier1(const ControlState &) = 0;

  /**
   * @brief Process action on tool modifier button no. 2
   *
   * @param[in] state	    State of button
   */
  virtual void onModifier2(const ControlState &) = 0;

  /**
   * @brief Process action on tool modifier button no. 3
   *
   * @param[in] state	    State of button
   */
  virtual void onModifier3(const ControlState &) = 0;

  /**
   * @brief Process moving of cursor
   *
   * @param[in] position	New position of cursor
   */
  virtual void onMove(const Vec2d &position) = 0;

  /**
   * @brief Confirm tool application
   */
  virtual void onConfirm(void) = 0;

  /**
   * @brief Cancel tool application
   */
  virtual void onCancel(void) = 0;

  /**
   * @brief Get tool preview & configuration widget
   *
   * @return Widget instance or `nullptr` if tool does not need preview or
   *         configuration
   */
  virtual Widget *getWidget() = 0;

  virtual ~Tool() = default;
};

} // namespace plug

// namespace plug
// {
//     enum State
//     {
//         Pressed, 
//         Released
//     };

//     struct ControlState 
//     {
//         State state = Released;
//     };

//     class Tool : public Plugin
//     {    
//     protected:
//         Widget *widget_ = nullptr;
//         ControlState state_;
//         ColorPalette *color_palette_ = nullptr;
//         Canvas *active_canvas_ = nullptr;

//     public:
//         virtual ~Tool () = default;

//         virtual void setColorPalette (ColorPalette &palette) = 0;
//         virtual void setActiveCanvas (Canvas &canvas) = 0; 	// храним канвас в самом туле чтобы не передавать его все время в качестве параметра и для того чтобы реагировать на изменение активного канваса.

//         virtual void on_main_button         (const ControlState &control_state, Vec2d &pos) = 0;
//         virtual void on_secondary_button    (const ControlState &control_state, Vec2d &pos) = 0;
//         virtual void on_modifier_1          (const ControlState &control_state) = 0;
//         virtual void on_modifier_2          (const ControlState &control_state) = 0;
//         virtual void on_modifier_3          (const ControlState &control_state) = 0;

//         virtual void on_move                (Vec2d &pos) = 0;
//         virtual void on_confirm             () = 0;
//         virtual void on_cancel              () = 0;
//         virtual void on_released_key        () = 0;

//         virtual Widget *get_widget () = 0;

//         virtual Plugin *tryGetInterface (size_t) = 0;	
//         virtual void addReference (Plugin *) = 0;
//         virtual void release (Plugin *) = 0;
//         virtual const PluginData *getPluginData () const = 0;
//     };
// }

#endif /* PLUG_TOOL_H */