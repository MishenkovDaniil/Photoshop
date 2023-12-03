#ifndef TEXT_WIDGET_H
#define TEXT_WIDGET_H

#include "../widget.h"

#include "../../graphics/rendertexture/rendertexture.h"
#include "../../graphics/rectangleshape/rectangleshape.h"
#include "../../graphics/font/font.h"
#include "../../graphics/text/text.h"

static const char *MONOSPACE_FONT_FILE = "resources/monospace_font.ttf";
static const size_t START_TEXT_CAPACITY = 512;
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
    Font text_font_;
    Text text_text_;
    RectangleShape  text_rect;
    plug::VertexArray cursor_line_ = plug::VertexArray (plug::Lines, 2);

public:
    M_text (plug::Vec2d lh_pos, int width, int height, plug::Color color = plug::Black);
    ~M_text ();

    const char *get_string () {assert (buf_); buf_[len_] = '\0'; return buf_;}

    void draw (plug::TransformStack &transform_stack, plug::RenderTarget &target) override; 
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) override;
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) override;
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) override;
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) override;
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;

private:
    char convert_key_to_char (plug::KeyCode key, plug::KeyCode latest_key_);
    char convert_key_to_char_shift (plug::KeyCode key, plug::KeyCode latest_key_);
    char convert_key_to_char_default (plug::KeyCode key, plug::KeyCode latest_key_);
    void run_managing_keys (plug::KeyCode key, plug::KeyCode latest_key_);

    void check_string ();
    void set_last_symbol (char symbol) {assert (buf_); buf_[len_] = symbol;};
};
//add len on picture
#endif /* TEXT_WIDGET_H */