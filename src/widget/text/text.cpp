#include "text.h"

M_text::M_text (Vector lh_pos, int width, int height, Color color) :
    text_color_ (color),
    capacity_ (START_TEXT_CAPACITY)
{
    layout_ = new Default_layout_box (lh_pos, Vector (width, height));
    assert (layout_ );
    buf_ = (char *)calloc (capacity_, sizeof (char));
    assert (buf_);
}

M_text::~M_text ()
{
    if (layout_) delete (layout_);
    if (buf_)    free (buf_);
    
    layout_ = nullptr;
    buf_ = nullptr;
}

void M_text::render (sf::RenderTarget &target, Transform_stack &transform_stack)
{
    Transform tr (layout_->get_position ());
    Transform unite = tr.unite (transform_stack.top ());

    Vector lh_pos = unite.offset_;
    Vector size   = layout_->get_size ();
    size = unite.scale_apply (size);
    

    sf::RectangleShape  text_rect (layout_->get_size ());
                        text_rect.setFillColor (Transparent);
                        text_rect.setOutlineColor (Black);
                        text_rect.setPosition (unite.offset_);
                        text_rect.setOutlineThickness (1);
    
    sf::String string (buf_);
    sf::Font    monospace_font;
                monospace_font.loadFromFile (MONOSPACE_FONT_FILE);
    sf::Text text (string, monospace_font, len_);
    text.setFillColor (text_color_);
    text.setCharacterSize (TEXT_CHARACTER_SIZE);

    double text_width = text.findCharacterPos(len_ - 1).x - text.findCharacterPos (0).x;
    text.setPosition (lh_pos);

    target.draw (text_rect);
    target.draw (text);
}   

bool M_text::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)
{
    is_pressed = true; // check containing
    return false;
}

bool M_text::on_mouse_released (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)
{   
    if (is_pressed)
    {
        is_pressed = false;
        return true;
    }

    return false;
}

bool M_text::on_mouse_moved    (Vector &new_pos, Transform_stack &transform_stack)
{
    return false;
}   

bool M_text::on_keyboard_pressed  (Keyboard_key key)
{
    // if (!is_pressed)
    //     return false;
    
    char sign = convert_key_to_char (key, latest_key_);
    latest_key_ = key;
    if (!sign)
        return true;


    int temp = 0;
    sprintf (buf_ + len_, "%c%n", sign, &temp);
    len_ += temp;
    
    return true;
}   

bool M_text::on_keyboard_released (Keyboard_key key)
{
    return false;
}   

bool M_text::on_tick (float delta_sec)
{
    return false;
}   

char M_text::convert_key_to_char (Keyboard_key key, Keyboard_key latest_key_)
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
            return '\n';
        case Backslash:
            return '\\';
        case Comma:
            return ',';
        case Period:
            return '.';
        case Backspace:
        {
            len_ = len_ ? --len_ : len_;
            *(buf_ + len_) = '\0';
        }
        default:
            return 0;    
    }
}