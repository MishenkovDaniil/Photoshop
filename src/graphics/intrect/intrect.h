#ifndef INTRECT_H
#define INTRECT_H

#include <SFML/Graphics.hpp>

class IntRect
{
    sf::IntRect rect_;
public:
    IntRect (int top, int left, size_t width, size_t height) : rect_ (top, left, width, height) {};
    IntRect () = default;
    ~IntRect () = default;

    IntRect operator = (const IntRect &other) {rect_ = other.rect_; return *this;};
    
    void setLeftCorner (int left) {rect_.left= left;};
    void setTopCorner (int top) {rect_.top= top;};
    void setWidth (size_t width) {rect_.width= width;};
    void setHeight (size_t height) {rect_.height= height;};

    int getLeftCorner () const {return rect_.left;};
    int getTopCorner () const {return rect_.top;};
    size_t getWidth () const {return rect_.width;};
    size_t getHeight () const {return rect_.height;;}; 
};

#endif /* INTRECT_H */