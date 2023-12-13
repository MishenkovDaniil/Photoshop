#ifndef PLUG_COLORPALETTE_H
#define PLUG_COLORPALETTE_H

#include "../graphic_structures/color/color.h"


namespace plug {

/**
 * @brief Palette of tool colors
 */
class ColorPalette {
public:
  /**
   * @brief Get active foreground color
   *
   * @return Active foreground color
   */
  virtual Color getFGColor(void) const = 0;

  /**
   * @brief Get active background color
   *
   * @return Active background color
   */
  virtual Color getBGColor(void) const = 0;

  /**
   * @brief Set new foreground color
   *
   * @param[in] new_fg_color  New value of background color
   */
  virtual void setFGColor(Color new_fg_color) = 0;

  /**
   * @brief Set new background color
   *
   * @param[in] new_bg_color  New value of background color
   */
  virtual void setBGColor(Color new_bg_color) = 0;

  virtual ~ColorPalette() {}
};

} // namespace plug

#endif /* PLUG_COLORPALETTE_H */