#include <iostream>
#include <cassert>

#include "../widget/widget.h"

void Window::render (sf::RenderTarget &target) const
{
    sf::RectangleShape window (sf::Vector2f (width_, height_));
    sf::RectangleShape window_frame (sf::Vector2f (width_ + 2 * thickness_, height_ + 2 * thickness_));
    
    window_frame.setFillColor ((sf::Color)frame_color_);
    window_frame.setFillColor ((sf::Color)w_color_);

    window.setPosition (lh_pos_.get_x (), lh_pos_.get_y ());
    window_frame.setPosition (lh_pos_.get_x () - thickness_, lh_pos_.get_y () - thickness_);

    target.draw (window_frame);
    target.draw (window);
}