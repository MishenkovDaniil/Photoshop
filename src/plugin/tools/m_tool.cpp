#include "m_tool.h"

#include "../../widget/texture_widget/standard_texture_widget.h"

extern "C" plug::Plugin *loadPlugin (size_t type)
{
    return ((plug::PluginGuid)type == plug::PluginGuid::Tool) ? new Rect_shape : nullptr;
}

plug::VertexArray MRectangle::getRect ()
{
    rect_[0].color =
    rect_[1].color =
    rect_[2].color =
    rect_[3].color =
    rect_[4].color =
    rect_[5].color =
    rect_[6].color = 
    rect_[7].color = color_;

    rect_[0].position = position_;
    rect_[1].position = position_ + plug::Vec2d (size_.x, 0);
    rect_[2].position = rect_[1].position;
    rect_[3].position = position_ + size_;
    rect_[4].position = rect_[3].position;
    rect_[5].position = position_ + plug::Vec2d (0, size_.y);
    rect_[6].position = rect_[5].position; 
    rect_[7].position = rect_[0].position;

    return rect_;
}


Rect_shape::Rect_shape () {};
Rect_shape::~Rect_shape () {};

void Rect_shape::onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos)
{
    assert (active_canvas_);

    if (control_state.state == plug::State::Pressed)
    {
        center_ = last_center_ = latest_pos_ = pos;
        
        plug::Vec2d canvas_size = active_canvas_->getSize ();
        widget_ = new PlugRenderTexture (canvas_size.get_x (), canvas_size.get_y (), plug::Transparent);
        assert (widget_);
        
        rect_.setSize (plug::Vec2d (0, 0));
        rect_.setPosition (center_);
        rect_.setColor (plug::Transparent);

        state_.state = plug::State::Pressed;
    }
}

void Rect_shape::onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos)
{
    return;
}

void Rect_shape::onModifier1          (const plug::ControlState &control_state)
{
    if (state_.state == plug::State::Released)
        return;
    
    assert (active_canvas_);

    is_on_modifier_1_ = true;

    PlugRenderTexture *draw_texture =  (PlugRenderTexture *)widget_;
    assert (draw_texture);
    draw_texture->clear (plug::Transparent);

    last_center_ = center_;
    plug::Vec2d pos_offset = latest_pos_ - center_;

    double min = 0;
    if (std::abs (pos_offset.get_x ()) < std::abs(pos_offset.get_y ()))
    {
        min = std::abs (pos_offset.get_x ());
    }
    else 
    {
        min = std::abs (pos_offset.get_y ());
    }

    rect_.setSize (plug::Vec2d (min, min));    

    if (pos_offset.get_x () < 0.0)
    {
        last_center_  -= plug::Vec2d (min, 0);
    }
    if (pos_offset.get_y () < 0.0)
    {
        last_center_  -= plug::Vec2d (0, min);
    }

    rect_.setPosition (last_center_);

    plug::VertexArray array = rect_.getRect ();
    draw_texture->draw (array);
}

void Rect_shape::onModifier2          (const plug::ControlState &control_state)
{
    return;
}

void Rect_shape::onModifier3          (const plug::ControlState &control_state)
{
    return;
}

void Rect_shape::onMove                (const plug::Vec2d &pos)
{
    if (state_.state == plug::State::Released)
        return;

    assert (active_canvas_);

    latest_pos_ = pos;
    
    if (is_on_modifier_1_)
    {
        plug::ControlState state;
        state.state = plug::State::Pressed;
        onModifier1 (state);
        return;
    }
    
    PlugRenderTexture *draw_texture =  (PlugRenderTexture *)widget_;
    assert (draw_texture);
    draw_texture->clear (plug::Transparent);

    last_center_ = center_;
    plug::Vec2d pos_offset = pos - center_;

    if (pos_offset.get_x () < 0.0)
    {
        last_center_  += plug::Vec2d (pos_offset.get_x (), 0);
        pos_offset = plug::Vec2d (std::abs (pos_offset.get_x ()), pos_offset.get_y ());
    }
    if (pos_offset.get_y () < 0.0)
    {
        last_center_  += plug::Vec2d (0, pos_offset.get_y ());
        pos_offset = plug::Vec2d (pos_offset.get_x (), std::abs (pos_offset.get_y ()));
    }
    
    rect_.setSize (pos_offset);    
    rect_.setPosition (last_center_);

    plug::VertexArray array = rect_.getRect ();
    draw_texture->draw (array);
}

void Rect_shape::onConfirm             ()
{
    if (state_.state == plug::State::Released)
        return;

    assert (active_canvas_);

    // plug::Texture texture = ((PlugRenderTexture *)widget_)->getTexture ();

    // plug::Vec2d canvas_size = active_canvas_->getSize ();
    // plug::VertexArray vertices (plug::Quads, 4);
    
    // vertices[0].position = plug::Vec2d (0, 0);
    // vertices[1].position = plug::Vec2d (0, texture.height);
    // vertices[2].position = plug::Vec2d (texture.width, texture.height);
    // vertices[3].position = plug::Vec2d (texture.width, 0);

    // vertices[0].color = plug::White;
    // vertices[1].color = plug::White;
    // vertices[2].color = plug::White;
    // vertices[3].color = plug::White;
    
    // vertices[0].tex_coords = plug::Vec2d (0, 0);
    // vertices[1].tex_coords = plug::Vec2d (0, texture.height - 1);
    // vertices[2].tex_coords = plug::Vec2d (texture.width - 1, texture.height - 1);
    // vertices[3].tex_coords = plug::Vec2d (texture.width - 1, 0);

    active_canvas_->draw (rect_.getRect ());

    // ((Canvas *)active_canvas_)->draw (rect_);

    state_.state = plug::State::Released;
    center_ = last_center_;
    
    delete widget_;
    widget_ = nullptr;
}

void Rect_shape::onCancel              ()
{
    if (state_.state == plug::State::Pressed)
    {
        delete widget_;
        widget_ = nullptr;
        state_.state = plug::State::Released;
    }
}