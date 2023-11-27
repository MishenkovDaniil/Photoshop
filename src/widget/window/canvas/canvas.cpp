#include "canvas.h"

bool SelectionMask::get_pixel (size_t x, size_t y) const
{
    assert (mask && "nullptr filter mask\n");
    assert ((x < width_) && (y < height_) && "invalid pixel coord\n");

    return mask[x + y * width_];
}  

void SelectionMask::set_pixel (size_t x, size_t y, bool flag)
{
    assert (mask && "nullptr filter mask\n");
    assert ((x < width_) && (y < height_) && "invalid pixel coord\n");

    mask[x + y * width_] = flag;
}  

void SelectionMask::fill (bool value)
{
    assert (mask && "nullptr filter mask\n");
    for (size_t idx = 0; idx < width_ * height_; ++idx)
    {
        mask[idx] = value;
    }
}

Canvas::Canvas (int width, int height, const plug::Color color) :
    width_ (width),
    height_ (height),
    color_ (color),
    selection (width, height),
    canvas_img (width, height * 2)
{
    if (width <= 0 || height <= 0)
    {
        fprintf (stderr, "Error: incorrect size of canvas.\n");
        return;
    }

    canvas_texture.create (width, height * 2);
    draw_rect_  = IntRect (0, 0, width, height);

    RectangleShape rect (plug::Vec2d (width, height));
    rect.setFillColor (color);
    
    canvas_texture.draw (rect, canvas_sprite);
    canvas_texture.display ();
    
    selection.fill (true); 
    plug::Texture texture = canvas_texture.getTexture ();
    memcpy (canvas_img.data, texture.data, sizeof (plug::Color) * texture.width * texture.height);
}

bool Canvas::contains (int x, int y)
{
    return ((x >= 0 && y >= 0) && 
            (x <= width_ && y <= height_));
}


void Canvas::setDrawRectOffset (int left, int top)
{
    if (left < 0 || top < 0)
    {
        fprintf (stderr, "Error: %s func has invalid offset parameters.\nHint: left = %d, top = %d.\n", __func__, left, top);
        return;
    }

    draw_rect_.setLeftCorner (left);
    draw_rect_.setTopCorner (top);
}

void Canvas::setDrawRectSize   (int width, int height)
{
    if (!(width > 0 && height > 0))
    {
        fprintf (stderr, "Error: %s func has invalid size parameters.\nHint: width = %d, height = %d.\n", __func__, width, height);
        return;
    }

    draw_rect_.setWidth (width);
    draw_rect_.setHeight (height);
}

plug::Color Canvas::getPixel(size_t x, size_t y) const
{
    x += draw_rect_.getLeftCorner ();
    y += draw_rect_.getTopCorner ();

    return canvas_img.data[x + y * width_];
}

void Canvas::setPixel(size_t x, size_t y, const plug::Color& color)
{
    x += draw_rect_.getLeftCorner ();
    y += draw_rect_.getTopCorner ();

    canvas_img.data[x + y * width_] = color;
}

const plug::Texture& Canvas::getTexture(void) const
{
    // plug::Texture texture = canvas_texture.getTexture ();
    if (is_changed_img)
    {
        plug::Texture texture = canvas_texture.getTexture ();
        memcpy (canvas_img.data, texture.data, sizeof (plug::Color) * texture.width * texture.height);
        // is_changed_img = false;
    }

    return canvas_img;
}

unsigned int Canvas::getNativeHandle(void) const
{
    //TODO;
    return 0;
}               

void Canvas::draw (const plug::VertexArray& vertex_array)                        
{
    // for (size_t idx = 0; idx < vertex_array.getSize (); ++idx)
    // {
    //     vertex_array[idx].position += plug::Vec2d (draw_rect_.getLeftCorner, draw_rect_.getTopCorner ());
    // }
    
    canvas_texture.draw (vertex_array, canvas_sprite);
    is_changed_img = true;
}   

void Canvas::draw (const plug::VertexArray& vertex_array, const plug::Texture &texture)
{
    canvas_texture.draw (vertex_array, texture, canvas_sprite);
    is_changed_img = true;
}

void Canvas::draw (const Drawable &drawable)
{
    canvas_texture.draw (drawable, canvas_sprite);
    is_changed_img = true;
}

CanvasView::CanvasView (int width, int height, const plug::Color color, const plug::Vec2d lh_pos, Tool_palette *palette) :
    view (width, height, color),
    palette_ (palette)
{
    if (width <= 0 || height <= 0)
    {
        fprintf (stderr, "Error: incorrect size of canvas.\n");
        return;
    }

    layout_ = new Default_layout_box (lh_pos, plug::Vec2d (width, height));
    assert (layout_);
}

CanvasView::~CanvasView () 
{
    // width_  = __INT_MAX__;
    // height_ = __INT_MAX__;
    delete layout_;
};

void CanvasView::render (plug::RenderTarget &target, plug::TransformStack &transform_stack)
{
    transform_stack.enter (plug::Transform (layout_->getPosition ()));
    plug::Vec2d lh_pos = transform_stack.top ().getOffset ();

    Sprite &canvas_sprite = view.canvas_sprite;
    canvas_sprite.setPosition (lh_pos);
    canvas_sprite.setTextureRect (view.draw_rect_);
     
    ((RenderTexture &)target).draw (canvas_sprite);
    ((RenderTexture &)target).display ();
    
    if (palette_ && is_focused)
    {
        plug::Tool *tool = palette_->get_cur_tool ();
        if (tool)
        {
            plug::Widget *widget = tool->get_widget ();
            if (widget)
            {
                widget->render (target, transform_stack);
            }
        }
    }

    transform_stack.leave ();
}

void CanvasView::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
{
    plug::Transform tr (layout_->getPosition ());
    plug::Transform unite = tr.combine (ehc.stack.top ());
    
    plug::Vec2d pos_ = unite.apply (event.pos);
    if (!contains (pos_.get_x (), pos_.get_y ()))
        return;
    
    if (!palette_)
    {
        return;
    }
    
    plug::Tool *tool = palette_->get_cur_tool ();
    if (tool)
    {
        if (tool->get_widget ())
        {
            // if (tool->get_widget ()->get_layout_box ().getPosition ())
            // transform_stack.enter (unite);
            
            tool->get_widget ()->onEvent (event, ehc);
            bool status = ehc.stopped;
            is_focused = status;
            // transform_stack.leave ();
            if (status) 
                return;
        }

        plug::ControlState control_state;
        control_state.state = plug::Pressed;

        tool->setActiveCanvas (view);
        tool->on_main_button (control_state, pos_);
        is_focused = true;
        ehc.stopped = true;

        return;
    }
}

void CanvasView::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    plug::Transform tr (layout_->getPosition ());
    plug::Transform unite = tr.combine (ehc.stack.top ()); // stack.enter + stack_top.apply
    plug::Vec2d pos_ = unite.apply (event.pos);

    if (!(palette_ && is_focused))
        return;
    
    plug::Tool *tool = palette_->get_cur_tool ();
    if (tool)
    {
        // ControlState control_state;
        // control_state.state = Released;

        tool->on_confirm ();
        if (!(tool->get_widget ())) is_focused = false;
        ehc.stopped = true;
    }
}

void CanvasView::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    plug::Transform tr (layout_->getPosition ());
    plug::Transform unite = tr.combine (ehc.stack.top ());

    plug::Vec2d pos_ = unite.apply (event.pos);

    if (!(palette_ && is_focused))
        return;
    
    plug::Tool *tool = palette_->get_cur_tool ();
    if (tool)
    {
        tool->on_move (pos_);
        // tool->on_modifier_1 (pos_, *this);
        ehc.stopped = true;
    }
}   

void CanvasView::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    if (!palette_)
        return;
    
    plug::Tool *tool = palette_->get_cur_tool ();

    if (tool) 
    {
        if (tool->get_widget ())
        {
            tool->get_widget ()->onEvent (event, ehc);
            bool status = ehc.stopped;
            is_focused = status;
            if (status) 
                return;
        }

        switch (event.key_id)
        {
            case plug::KeyCode::Escape:
            {
                tool->on_cancel ();
                ehc.stopped = true;
                return;
            }
            case plug::KeyCode::Enter:
            {
                tool->on_confirm ();
                ehc.stopped = true;
                return;
            }
            case plug::KeyCode::RShift:
            case plug::KeyCode::LShift:
            {
                plug::ControlState state = {plug::Pressed};
                tool->on_modifier_1 (state);
                is_focused = true;
                ehc.stopped = true;
                return;
            }
            default:
            {
                break;
            }
        }
    }
}

void CanvasView::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    if (!palette_)
    {
        return;
    }

    plug::Tool *tool = palette_->get_cur_tool ();
    if (!tool)
    {
        return;
    }

    tool->on_released_key ();
    ehc.stopped = true;
}

void CanvasView::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
{
    if (!palette_)
    {
        return;
    }

    plug::Tool *tool = palette_->get_cur_tool ();
    if (!(tool && tool->get_widget ()))
    {
        return;
    }

    tool->get_widget ()->onEvent (event, ehc);
}

bool CanvasView::contains (int x, int y)
{
    // return ((x >= 0 && y >= 0) && 
            // (x <= width_ && y <= height_));
    return view.contains (x, y);
}

void CanvasView::setDrawRectOffset (int left, int top)
{
    // if (left < 0 || top < 0)
    // {
    //     fprintf (stderr, "Error: %s func has invalid offset parameters.\nHint: left = %d, top = %d.\n", __func__, left, top);
    //     return;
    // }

    // draw_rect_.left  = left;
    // draw_rect_.top = top;
    view.setDrawRectOffset (left, top);
}

void CanvasView::setDrawRectSize (int width, int height)
{
    // if (!(width > 0 && height > 0))
    // {
    //     fprintf (stderr, "Error: %s func has invalid size parameters.\nHint: width = %d, height = %d.\n", __func__, width, height);
    //     return;
    // }

    // draw_rect_.width  = width;
    // draw_rect_.height = height;
    view.setDrawRectSize (width, height);
}