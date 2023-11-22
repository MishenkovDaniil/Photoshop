#include "menu.h"


Menu::Menu (plug::Vec2d lh_pos, int width, int height) :
    width_ (width),
    height_ (height)
{
    layout_ = new Default_layout_box (lh_pos, plug::Vec2d (width, height));
    assert (layout_);
}

Menu::~Menu ()
{
    delete layout_;
}


void Menu::add_button (Button *button)
{
    assert (button && "nullptr button added to menu");
    
    buttons.add (button);
}

void Menu::render (sf::RenderTarget &target, plug::TransformStack &transform_stack) 
{
    transform_stack.enter (plug::Transform (layout_->getPosition ()));

    plug::Vec2d lh_pos = transform_stack.top ().getOffset ();

    sf::RectangleShape rect (plug::Vec2d (width_, height_));
    rect.setFillColor    (plug::Color (200, 200, 200));
    rect.setOutlineColor (plug::Color (50, 50, 50));
    rect.setOutlineThickness (-1);
    rect.setPosition (lh_pos + plug::Vec2d (0, -1));
    target.draw (rect);


    size_t button_num = buttons.get_size ();
    for (int button_idx = 0; button_idx < button_num; ++button_idx)
    {
        Button *button = buttons[button_idx];
        if (!button)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in buttons vector = %d.\n"
                             "Hint: buttons vector size = %lu.\n", button_idx, button_num);
            return;
        }
        assert (button);

        button->render (target, transform_stack);
    }

    transform_stack.leave ();
}

void Menu::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));
    size_t button_num = buttons.get_size ();
    for (int button_idx = 0; button_idx < button_num; ++button_idx)
    {
        Button *button = buttons[button_idx];
        if (!button)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in buttons vector = %d.\n"
                             "Hint: buttons vector size = %lu.\n", button_idx, button_num);
            ehc.stopped = true;
            return;
        }
        assert (button);

        button->onMousePressed (event, ehc);
    }

    ehc.stack.leave ();
}

void Menu::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));

    size_t button_num = buttons.get_size ();
    for (int button_idx = 0; button_idx < button_num; ++button_idx)
    {
        Button *button = buttons[button_idx];
        if (!button)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in buttons vector = %d.\n"
                             "Hint: buttons vector size = %lu.\n", button_idx, button_num);
            ehc.stopped = true;
            return;
        }
        assert (button);

        button->onMouseReleased (event, ehc);
    }

    ehc.stack.leave ();
}

void Menu::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));

    size_t button_num = buttons.get_size ();
    for (int button_idx = 0; button_idx < button_num; ++button_idx)
    {
        Button *button = buttons[button_idx];
        if (!button)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in buttons vector = %d.\n"
                             "Hint: buttons vector size = %lu.\n", button_idx, button_num);
            ehc.stopped = true;
            return;
        }
        assert (button);

        button->onMouseMove (event, ehc);
    }

    ehc.stack.leave ();
}

void Menu::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    return;
}

void Menu::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    return;
}

void Menu::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
{
    return;
}
