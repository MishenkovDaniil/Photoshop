#ifndef PLUG_FILTER_H 
#define PLUG_FILTER_H

#include "plug_plugin.h"
#include "plug_widget.h"
#include "plug_canvas.h"


namespace plug {

/**
 * @brief Interface for pixel filter plugin
 */
class Filter : public Plugin {
public:
  /**
   * @brief Apply filter to canvas
   *
   * @param[inout] canvas	Target canvas
   */
  virtual void applyFilter(Canvas &canvas) const = 0;

  /**
   * @brief Retrieve filter configuration widget
   *
   * @return Widget instance of `nullptr` if filter does not require
   *         configuration
   */
  virtual Widget *getWidget(void) = 0;

  virtual ~Filter() override = default;
};

} // namespace plug


// namespace plug
// {
//     class Filter : public Plugin
//     {
//     public:
//         virtual ~Filter () = default;

//         virtual void apply_filter (Canvas &canvas) const = 0;
//         virtual Widget *getWidget () = 0;

//         virtual Plugin *tryGetInterface (size_t) = 0;	
//         virtual void addReference (Plugin *) = 0;
//         virtual void release (Plugin *) = 0;
//         virtual const PluginData *getPluginData () const = 0;
//     };
// }

#endif /* PLUG_FILTER_H */