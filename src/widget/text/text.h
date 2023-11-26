#ifndef TEXT_WIDGET_H
#define TEXT_WIDGET_H

#include "../widget.h"

static const size_t START_TEXT_CAPACITY = 512;
static const char *MONOSPACE_FONT_FILE = "resources/monospace_font.ttf";
static const size_t TEXT_CHARACTER_SIZE = 14;
static const double TEXT_CHARACTER_WIDTH = 8.4; //hardcode!!!

class M_text : public Widget
{
    char  *buf_ = nullptr;
    size_t letters_in_string_ = 0;
    size_t len_ = 0;
    size_t capacity_ = 0;
    
    const size_t string_size_ = 0;
    
    bool is_pressed = false;
    bool is_filled_ = false;
    size_t cur_height = 0;
    plug::Color  text_color_ = plug::Transparent;
    plug::KeyCode latest_key_ = plug::KeyCode::Unknown;

    size_t cursor_pos = 0;

public:
    M_text (plug::Vec2d lh_pos, int width, int height, plug::Color color = plug::Black);
    ~M_text ();

    const char *get_string () {assert (buf_); buf_[len_] = '\0'; return buf_;}

    void render (plug::RenderTarget &target, plug::TransformStack &transform_stack) override; 
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) override;
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) override;
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) override;
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) override;
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;

    char convert_key_to_char (plug::KeyCode key, plug::KeyCode latest_key_);
    void check_string ();
    void set_last_symbol (char symbol) {assert (buf_); buf_[len_] = symbol;};
};

#endif /* TEXT_WIDGET_H */