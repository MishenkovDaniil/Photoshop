#ifndef PLUG_TEXTURE_H
#define PLUG_TEXTURE_H

#include <iostream>

#include "../graphic_structures/color/color.h"


namespace plug {

/**
 * @brief Array of colors (stored on the CPU)
 *
 */
struct Texture {
  Texture(size_t a_width, size_t a_height)
      : data((Color *)new Color[a_width * a_height]), width(a_width),
        height(a_height) {}

  Texture(size_t a_width, size_t a_height, const Color *a_data)
      : Texture(a_width, a_height) {
    for (size_t i = 0; i < width * height; ++i) {
      data[i] = a_data[i];
    }
  }

  Texture(const Texture &other)
      : Texture(other.width, other.height, other.data) {}

  Texture &operator=(const Texture &other) = delete;

  ~Texture(void) { delete[] data; }

  plug::Color getPixel(size_t x, size_t y) const {
    assert(x < width);
    assert(y < height);
    return data[y * width + x];
  }

  void setPixel(size_t x, size_t y, plug::Color color) const {
    assert(x < width);
    assert(y < height);
    data[y * width + x] = color;
  }

  /**
   * @brief Image data
   */
  Color *const data;

  /**
   * @brief Image width (in pixels)
   */
  const size_t width;

  /**
   * @brief Image height (in pixels)
   */
  const size_t height;
};

} // namespace plug

// namespace plug
// {
//     struct Texture
//     {
//         Color* data = nullptr;
//         const size_t width;
//         const size_t height;

//         Texture (size_t width, size_t height, Color* const data) :
//             data(data), width(width), height(height) {};

//         Texture (size_t width, size_t height) :
//             data((Color*)new Color[width * height]), width(width), height(height) {};           
        
//         ~Texture () {if (data)
//                     {
//                         delete[] data;
//                         data = nullptr;
//                     }};
//     };
// }

#endif /* PLUG_TEXTURE_H */