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

#endif /* PLUG_FILTER_H */