#ifndef TEXT_WIDGET_H
#define TEXT_WIDGET_H

#include "../widget.h"

static const size_t START_TEXT_CAPACITY = 512;
static const char *MONOSPACE_FONT_FILE = "resources/monospace_font.ttf";
static const size_t TEXT_CHARACTER_SIZE = 14;
static const double TEXT_CHARACTER_WIDTH = 8.4; //hardcode!!!

class M_text : public Widget
{
    char *buf_ = nullptr;
    Color text_color_ = Transparent;
    size_t len_ = 0;
    size_t capacity_ = 0;
    bool is_pressed;
    Keyboard_key latest_key_ = Unknown;
    size_t string_size_ = 0;
    size_t letters_in_string_ = 0;
    size_t cur_height = 0;
    bool is_filled_ = false;

public:
    M_text (Vector lh_pos, int width, int height, Color color = Black);
    ~M_text ();

    void render (sf::RenderTarget &target, Transform_stack &transform_stack)                    override; 
    bool on_mouse_pressed  (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack) override;
    bool on_mouse_released (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack) override;
    bool on_mouse_moved    (Vector &new_pos, Transform_stack &transform_stack)                  override;
    bool on_keyboard_pressed  (Keyboard_key key)                                                override;
    bool on_keyboard_released (Keyboard_key key)                                                override;
    bool on_tick (float delta_sec)                                                              override;
    char convert_key_to_char (Keyboard_key key, Keyboard_key latest_key_);
    void check_string ();
    void set_last_symbol (char symbol) {assert (buf_); buf_[len_] = symbol;};
};


#endif /* TEXT_WIDGET_H */