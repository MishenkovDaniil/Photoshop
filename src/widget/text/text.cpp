#include "text.h"

plug::M_text::M_text (plug::Vec2d lh_pos, int width, int height, plug::Color color) :
    text_color_ (color),
    capacity_ (START_TEXT_CAPACITY),
    string_size_ ((double)width / TEXT_CHARACTER_WIDTH),
    cur_height (TEXT_CHARACTER_SIZE)
{
    layout_ = new plug::Default_layout_box (lh_pos, plug::Vec2d (width, height));
    assert (layout_ );
    buf_ = (char *)calloc (capacity_, sizeof (char));
    assert (buf_);
}

plug::M_text::~M_text ()
{
    if (layout_) delete (layout_);
    if (buf_)    free (buf_);
    
    layout_ = nullptr;
    buf_ = nullptr;
}

void plug::M_text::render (sf::RenderTarget &target, TransformStack &transform_stack)
{
    static size_t a = 0;
    static char b = '|';
    static char c = '\0';

    if (!(a % 50))
    {
        buf_[cursor_pos] = b;
        std::swap (b, c);
    }
    a++;   
    
    plug::Transform tr (layout_->get_position ());
    plug::Transform unite = tr.combine (transform_stack.top ());

    plug::Vec2d lh_pos = unite.getOffset ();
    plug::Vec2d size   = layout_->get_size ();
    // size = unite.scale_apply (size);
    
    sf::RectangleShape  text_rect (layout_->get_size ());
                        text_rect.setFillColor (Transparent);
                        text_rect.setOutlineColor (Black);
                        text_rect.setPosition (unite.getOffset ());
                        text_rect.setOutlineThickness (1);
    
    sf::String string (buf_);
    sf::Font    monospace_font;
                monospace_font.loadFromFile (MONOSPACE_FONT_FILE);
    sf::Text text;
    text.setString (string);
    text.setFont (monospace_font);
    text.setFillColor (text_color_);
    text.setCharacterSize (TEXT_CHARACTER_SIZE);
    text.setPosition (lh_pos);
    
    target.draw (text_rect);
    target.draw (text);
}   

void plug::M_text::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
{
    is_pressed = true; // check containing

    // Transform unite = Transform (layout_->get_position ()).unite (transform_stack.top ());

    // Vec2d pos_ = unite.apply_transform (pos);
    // printf ("pos = %lf, %lf\n", pos.get_x (), pos.get_y ());
    // printf ("pos_ = %lf, %lf\n", pos_.get_x (), pos_.get_y ());
    // cursor_pos = ((size_t)pos_.get_y () / TEXT_CHARACTER_SIZE) * string_size_ + (size_t)pos_.get_y () / TEXT_CHARACTER_WIDTH;
    
    // printf ("cursor_pos = %lu\n", cursor_pos);
    ehc.stopped = true;
    return;
}

void plug::M_text::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    if (is_pressed)
    {
        is_pressed = false;
        ehc.stopped = true;
    }
}

void plug::M_text::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    return;
}

void plug::M_text::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    // if (!is_pressed)
    //     return false;

    char sign = convert_key_to_char (event.key_id, latest_key_);
    latest_key_ = event.key_id;
    if (!sign)
    {
        ehc.stopped = true;
        return;
    }


    sprintf (buf_ + len_, "%c", sign);
    ++len_;
    cursor_pos++;
    check_string ();

    ++letters_in_string_;
    ehc.stopped = true;
}

void plug::M_text::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    return;
}

void plug::M_text::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
{
    static size_t a = 0;
    static char b = '|';
    static char c = '\0';

    if (!(a % 50))
    {
        buf_[len_] = b;
        std::swap (b, c);
    }
    a++;   

    ehc.stopped = true;
}



char plug::M_text::convert_key_to_char (plug::KeyCode key, plug::KeyCode latest_key_)
{
    if (key >= A && key <= Z)
    {
        if (latest_key_ == LShift || latest_key_ == RShift)
            return 'A' + key - A;
        else 
            return 'a' + key - A;
    }
    else if (key >= Num0 && key <= Num9)
    {
        if (!(latest_key_ == LShift || latest_key_ == RShift))
            return '0' + key - Num0;
    }

    switch (key)
    {
        case Enter:
        {
            letters_in_string_ = 0;
            return '\n';
        }
        case Backslash:
            return '\\';
        case Comma:
            return ',';
        case Period:
            return '.';
        case Backspace:
        {
            letters_in_string_ = letters_in_string_ ? --letters_in_string_: letters_in_string_;
            len_ = len_ ? --len_ : len_;
            *(buf_ + len_) = '\0';
        }
        case Space:
        {
            return ' ';
        }
        default:
            return 0;    
    }
}

void plug::M_text::check_string ()
{
    if (letters_in_string_ > string_size_)
    {
        buf_[len_] = buf_[len_ - 1];
        buf_[len_ - 1] = '\n';
        ++len_;
        letters_in_string_ = 0;
        cur_height += TEXT_CHARACTER_SIZE;
    }
    else if (buf_[len_ - 1] == '\n')
    {
        cur_height += TEXT_CHARACTER_SIZE;
    }

    if (is_filled_ == false && cur_height > layout_->get_size ().get_y ())
    {
        buf_[len_ - 1] = '\0';
        len_++;
        is_filled_ = true;
    }
}

