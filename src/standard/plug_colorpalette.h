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

// namespace plug 
// {
//     class ColorPalette 
//     {
//         Color fg_color_ = Red;
//         Color bg_color_ = White;
//     public:
//         Color getFGColor () const {return fg_color_;}; 
//         Color getBGColor () const {return bg_color_;}; 
//         void setFGColor (Color fg_color) {fg_color_ = fg_color;};
//         void setBGColor (Color bg_color) {bg_color_ = bg_color;};
//     };
// }

#endif /* PLUG_COLORPALETTE_H */