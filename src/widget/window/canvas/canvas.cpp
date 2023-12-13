#include "canvas.h"

bool SelectionMask::getPixel (size_t x, size_t y) const
{
    assert (mask && "nullptr filter mask\n");
    assert ((x < width_) && (y < height_) && "invalid pixel coord\n");

    return mask[x + y * width_];
}  

void SelectionMask::setPixel (size_t x, size_t y, bool flag)
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

    RectangleShape rect (plug::Vec2d (width, height * 2));
    rect.setFillColor (color);
    
    canvas_texture.draw (rect, canvas_sprite);
    canvas_texture.display ();
    
    selection.fill (true); 
    plug::Texture texture = canvas_texture.getTexture ();
    memcpy (canvas_img.data, texture.data, sizeof (plug::Color) * texture.width * texture.height);
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
    is_changed_img = true;
}

const plug::Texture& Canvas::getTexture(void) const
{
    // plug::Texture texture = canvas_texture.getTexture ();
    if (is_changed_texture)
    {
        plug::Texture texture = canvas_texture.getTexture ();

        memcpy (canvas_img.data, texture.data, sizeof (plug::Color) * texture.width * texture.height);
        // is_changed_img = false;
    }

    return canvas_img;
}

void Canvas::draw (const plug::VertexArray& vertex_array)                        
{
    // for (size_t idx = 0; idx < vertex_array.getSize (); ++idx)
    // {
    //     vertex_array[idx].position += plug::Vec2d (draw_rect_.getLeftCorner, draw_rect_.getTopCorner ());
    // }
    
    update_texture ();
    canvas_texture.draw (vertex_array, canvas_sprite);
    is_changed_texture = true;
    update_img ();
}  

void Canvas::update_texture ()
{
    if (is_changed_img)
    {
        plug::VertexArray vertices (plug::Quads, 4);

        vertices[0].position = plug::Vec2d (0, 0);
        vertices[1].position = plug::Vec2d (width_, 0);
        vertices[2].position = plug::Vec2d (width_, height_);
        vertices[3].position = plug::Vec2d (0, height_);
        // Sprite sprite (canvas.getTexture ());
        vertices[0].color = getPixel (0, 0);
        vertices[1].color = getPixel (width_ - 1, 0);
        vertices[2].color = getPixel (width_ - 1, height_ - 1);
        vertices[3].color = getPixel (0, height_ - 1);

        // for (int i = 0; i < 4; ++i)
        // {
        //     printf ("vertex color = %d %d %d %d\n", vertices[i].color.r, vertices[i].color.g, vertices[i].color.b, vertices[i].color.a);
        // }

        vertices[0].tex_coords = plug::Vec2d (0, 0);
        vertices[1].tex_coords = plug::Vec2d (width_ - 1, 0);
        vertices[2].tex_coords = plug::Vec2d (width_ - 1, height_ - 1);
        vertices[3].tex_coords = plug::Vec2d (0, height_ - 1);
        canvas_texture.draw (vertices, canvas_img);
        is_changed_img = false;
    }
}

void Canvas::update_img ()
{
    if (is_changed_texture)
    {
        canvas_texture.getTexture (canvas_img);
        is_changed_texture = false;
    }
}

void Canvas::dump_img ()
{
    for (size_t y = 1; y < 2; ++y)
    {
        for (size_t x = 0; x < 50; ++x)
        {
            printf ("[%lu][%lu] = %d %d %d %d\n", x, y, canvas_img.data[x + y * width_].r,
                                                      canvas_img.data[x + y * width_].g,
                                                      canvas_img.data[x + y * width_].b, 
                                                      canvas_img.data[x + y * width_].a);
        }
    }
    printf ("\n\n\n\n\n");
}

void Canvas::draw (const plug::VertexArray& vertex_array, const plug::Texture &texture)
{
    update_texture ();

    canvas_texture.draw (vertex_array, texture, canvas_sprite);
    is_changed_texture = true;
    update_img ();
}

void Canvas::draw (Drawable &drawable)
{
    update_texture ();

    plug::Vec2d real_pos = drawable.getPosition () + plug::Vec2d (draw_rect_.getLeftCorner (), draw_rect_.getTopCorner ());
    drawable.setPosition (real_pos);

    canvas_texture.draw (drawable, canvas_sprite);
    is_changed_texture = true;

    update_img ();
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

void CanvasView::draw (plug::TransformStack &transform_stack, plug::RenderTarget &target)
{
    transform_stack.enter (plug::Transform (layout_->getPosition ()));
    plug::Vec2d lh_pos = transform_stack.top ().getOffset ();
    //  TODO add draw rect to Danya preview 

    view.update_texture ();

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
            plug::Widget *widget = tool->getWidget ();
            if (widget)
            {
                // transform_stack.enter (plug::Transform (plug::Vec2d (view.getDrawRect ().getLeftCorner (), view.getDrawRect ().getTopCorner ())));
                widget->draw (transform_stack, target);
                // transform_stack.leave ();
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
    
    if (!covers (ehc.stack, event.pos))
        return;
    
    if (!palette_)
    {
        return;
    }
    
    plug::Tool *tool = palette_->get_cur_tool ();
    if (tool)
    {
        if (tool->getWidget ())
        {
            // if (tool->get_widget ()->get_layout_box ().getPosition ())
            // transform_stack.enter (unite);
            
            // ehc.stack.enter (plug::Transform (plug::Vec2d (view.getDrawRect ().getWidth (), view.getDrawRect ().getHeight ())));
            tool->getWidget ()->onEvent (event, ehc);
            // ehc.stack.leave ();
            bool status = ehc.stopped;
            is_focused = status;
            // transform_stack.leave ();
            if (status) 
                return;
        }

        plug::ControlState control_state;
        control_state.state = plug::State::Pressed;

        tool->setActiveCanvas (view);
        tool->onMainButton (control_state, pos_);
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

        tool->onConfirm ();
        if (!(tool->getWidget ())) is_focused = false;
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
        tool->onMove (pos_);
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
        if (tool->getWidget ())
        {
            tool->getWidget ()->onEvent (event, ehc);
            bool status = ehc.stopped;
            // ehc.stopped = true;
            is_focused = status;
            if (status) 
                return;
        }

        switch (event.key_id)
        {
            case plug::KeyCode::Escape:
            {
                tool->onCancel ();
                ehc.stopped = true;
                printf ("cancel\n");
                return;
            }
            case plug::KeyCode::Enter:
            {
                tool->onConfirm ();
                ehc.stopped = true;
                return;
            }
            case plug::KeyCode::RShift:
            case plug::KeyCode::LShift:
            {
                plug::ControlState state = {plug::State::Pressed};
                tool->onModifier1 (state);
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

    // tool->on_released_key ();
    ehc.stopped = true;
}

void CanvasView::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
{
    if (!palette_)
    {
        return;
    }

    plug::Tool *tool = palette_->get_cur_tool ();
    if (!(tool && tool->getWidget ()))
    {
        return;
    }

    tool->getWidget ()->onEvent (event, ehc);
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