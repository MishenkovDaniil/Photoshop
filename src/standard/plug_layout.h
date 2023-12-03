#ifndef PLUG_LAYOUT_H
#define PLUG_LAYOUT_H

#include "../graphic_structures/vector/vector.h"



namespace plug {

/**
 * @brief Representation of widget move and resize policy
 */
class LayoutBox {
public:
  /**
   * @brief Update information about parent layout box
   *
   * @param[in] parent_box  Parent layout box
   */
  virtual void onParentUpdate(const LayoutBox &parent_box) = 0;

  /**
   * @brief Get current size (in screen units) of layout box
   *
   * @return Size of layout box in screen units
   */
  virtual Vec2d getSize(void) const = 0;

  /**
   * @brief Get position of layout box (in screen units) relative to parent box
   *
   * @return Relative position of layout box in screen units
   */
  virtual Vec2d getPosition(void) const = 0;

  /**
   * @brief Update size of layout box
   *
   * @param[in] size	New size of layout box (in screen units)
   *
   * @return `true` if layout box size was updated, `false` otherwise
   */
  virtual bool setSize(const Vec2d &size) = 0;

  /**
   * @brief Update relative position of layout box
   *
   * @param[in] size	New position of layout box relative to parent
   *                  (in screen units)
   *
   * @return `true` if layout box position was updated, `false` otherwise
   */
  virtual bool setPosition(const Vec2d &position) = 0;

  /**
   * @brief Create deep copy of layout box
   *
   * @return Pointer to allocated copy of layout box
   */
  virtual LayoutBox *clone(void) const = 0;

  virtual ~LayoutBox() {}
};

} // namespace plug

// namespace plug
// {
//     class LayoutBox
//     {
//     public:
//         virtual ~LayoutBox () = default;
//         virtual void onParentUpdate   (const LayoutBox &parent_layout) = 0;
//         virtual Vec2d getPosition      () const = 0;
//         virtual Vec2d getSize          () const = 0;
//         virtual LayoutBox *clone       () const = 0;
//         virtual bool setPosition       (const Vec2d &new_pos) = 0;
//         virtual bool setSize           (const Vec2d &new_size) = 0;
//     };
// }

#endif /* PLUG_LAYOUT_H */