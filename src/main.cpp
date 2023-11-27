#include <cassert>
#include <ctime>
#include <dlfcn.h>
#include <iostream>

#include "graphics/sprite/sfml_texture.h"
#include "plugin/tools/tools.h"
#include "plugin/tools/palette/palette.h"
#include "plugin/filter/filter.h"
#include "widget/button/button.h"
#include "widget/button/clock/clock.h"
#include "widget/button/palette/palette.h"
#include "widget/widget.h"
#include "widget/widget_manager/widget_manager.h"
#include "widget/window/canvas/canvas.h"
#include "widget/window/master_window/master_window.h"
#include "widget/window/menu/menu.h"
#include "widget/window/window.h"

#include "standard/standard.h"

static const char *FILTER_PATH = "/home/daniil/programming/code/C++_projects/photoshop/libFilter.so";

static const char *line_img   = "resources/imgs/line.png";
static const char *fill_img   = "resources/imgs/fill.png";
static const char *brush_img  = "resources/imgs/brush.png";
static const char *circle_img = "resources/imgs/circle.png";
static const char *rect_img   = "resources/imgs/rect.png";
static const char *text_img   = "resources/imgs/text.png";

static const int FULL_SCREEN_WIDTH = 1920;
static const int FULL_SCREEN_HEIGHT = 1080;
static const int LIGHT_DELTA_CHANGE = 5;
static const int SATURATION_DELTA_CHANGE = 5;

typedef plug::Plugin *(getInterfaceFunc) (size_t type);	/// аналог  QueryInterface

bool save_file (void *widget, void *arg);

plug::Filter *getFilter (const char *plugin_path);

int main ()
{
    sf::RenderWindow window (sf::VideoMode (FULL_SCREEN_WIDTH, FULL_SCREEN_HEIGHT), "w_manager");//, sf::Style::Fullscreen);
    sf::Vector2u window_size = window.getSize ();
    
    RenderTexture render_texture;
    render_texture.create (window_size.x, window_size.y);

    plug::Vec2d pos (50, 50);

    Widget_manager widget_manager; 

    Tool_palette palette;
    Master_window main_window (window_size.x, window_size.y, plug::Vec2d (0, 0), "master");
    
    Clock clock_button (plug::Vec2d (0, 0), 100, 30, nullptr, &main_window, 10, 10, 55, nullptr);
    widget_manager.add_widget (&main_window);
    widget_manager.add_widget (&clock_button);

    Window child_window (600, 600, plug::Vec2d (200, 200), "window_1");
    Window child_window_2 (600, 600, plug::Vec2d (600, 600), "window_2");
    main_window.add_window (&child_window);
    main_window.add_window (&child_window_2);

    Sprite window_sprite;
    bool status = true;

    sf::Clock clock;
    float delta_clock = 0;

    time_t time_;
    struct tm *time_info;
    time (&time_);
    time_info = localtime (&time_);

    clock_button.change_time (time_info->tm_sec, time_info->tm_min, time_info->tm_hour);

    
    class Brush brush_tool; 
    Line line_tool; 
    Circle_shape circle_tool; 
    Rect_shape rect_tool; 
    Fill fill_tool; 
    Text_tool text_tool;

    palette.add_tool (&brush_tool);
    palette.add_tool (&line_tool);
    palette.add_tool (&circle_tool);
    palette.add_tool (&rect_tool);
    palette.add_tool (&fill_tool);
    palette.add_tool (&text_tool);

    Button_palette button_palette (plug::Vec2d (1650, 50), 200, 200, &palette);
    
    Light_filter light_incr (LIGHT_DELTA_CHANGE);
    Light_filter light_decr (-LIGHT_DELTA_CHANGE);
    Saturation_filter saturation_incr (SATURATION_DELTA_CHANGE);
    Saturation_filter saturation_decr (-SATURATION_DELTA_CHANGE);
    // White_black_filter black_white;
    plug::Filter *loaded_filter = getFilter (FILTER_PATH);  

    Filter_tool light_incr_tool (&light_incr);
    Filter_tool light_decr_tool (&light_decr);
    Filter_tool saturation_incr_tool (&saturation_incr);
    Filter_tool saturation_decr_tool (&saturation_decr);
    // Filter_tool black_white_tool (&black_white);
    Filter_tool loaded_filter_tool (loaded_filter);

    Sprite circle_pressed_texture;
    Sprite  brush_pressed_texture;
    Sprite   text_pressed_texture;
    Sprite   rect_pressed_texture;
    Sprite   line_pressed_texture;
    Sprite   fill_pressed_texture;
    Sprite circle_released_texture;
    Sprite  brush_released_texture;
    Sprite   rect_released_texture;
    Sprite   text_released_texture;
    Sprite   line_released_texture;
    Sprite   fill_released_texture;

    circle_released_texture.loadFromFile (circle_img);
     brush_released_texture.loadFromFile (brush_img);
      line_released_texture.loadFromFile (line_img);
      fill_released_texture.loadFromFile (fill_img);
      rect_released_texture.loadFromFile (rect_img);
      text_released_texture.loadFromFile (text_img);
     circle_pressed_texture.loadFromFile (circle_img);
      brush_pressed_texture.loadFromFile (brush_img);
       line_pressed_texture.loadFromFile (line_img);
       text_pressed_texture.loadFromFile (text_img);
       rect_pressed_texture.loadFromFile (rect_img);
       fill_pressed_texture.loadFromFile (fill_img);

    Pair light_incr_args      = Pair((void *)&palette, &light_incr_tool);
    Pair light_decr_args      = Pair((void *)&palette, &light_decr_tool);
    Pair saturation_incr_args = Pair((void *)&palette, &saturation_incr_tool);
    Pair saturation_decr_args = Pair((void *)&palette, &saturation_decr_tool);
    // Pair black_white_args     = Pair((void *)&palette, &black_white_tool);
    Pair loaded_filter_args     = Pair((void *)&palette, &loaded_filter_tool);

    Button red_button    (plug::Vec2d (0, 160),  20, 20, color_button_run_fn, (void *)&main_window, (void *)&plug::Red,    plug::Red,    PRESS_BUTTON);
    Button blue_button   (plug::Vec2d (20, 160), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&plug::Blue,   plug::Blue,   PRESS_BUTTON);
    Button green_button  (plug::Vec2d (40, 160), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&plug::Green,  plug::Green,  PRESS_BUTTON);
    Button white_button  (plug::Vec2d (60, 160), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&plug::White,  plug::White,  PRESS_BUTTON);
    Button black_button  (plug::Vec2d (80, 160), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&plug::Black,  plug::Black,  PRESS_BUTTON);
    Button cyan_button   (plug::Vec2d (0, 180),  20, 20, color_button_run_fn, (void *)&main_window, (void *)&plug::Cyan,   plug::Cyan,   PRESS_BUTTON);
    Button purple_button (plug::Vec2d (20, 180), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&plug::Purple, plug::Purple, PRESS_BUTTON);
    Button yellow_button (plug::Vec2d (40, 180), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&plug::Yellow, plug::Yellow, PRESS_BUTTON);
    Button brown_button  (plug::Vec2d (60, 180), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&plug::Brown,  plug::Brown,  PRESS_BUTTON);
    Button maroon_button (plug::Vec2d (80, 180), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&plug::Maroon, plug::Maroon, PRESS_BUTTON);
    
    Texture_button brush_button  (plug::Vec2d (0, 0),   50, 50, brush_pressed_texture,  brush_released_texture,  tool_run_fn, (void *)&main_window, (void *)&brush_tool,  PRESS_BUTTON);
    Texture_button line_button   (plug::Vec2d (50, 0),  50, 50, line_pressed_texture,   line_released_texture,   tool_run_fn, (void *)&main_window, (void *)&line_tool,   PRESS_BUTTON);
    Texture_button circle_button (plug::Vec2d (100, 0), 50, 50, circle_pressed_texture, circle_released_texture, tool_run_fn, (void *)&main_window, (void *)&circle_tool, PRESS_BUTTON);
    Texture_button rect_button   (plug::Vec2d (150, 0), 50, 50, rect_pressed_texture,   rect_released_texture,   tool_run_fn, (void *)&main_window, (void *)&rect_tool,   PRESS_BUTTON);
    Texture_button fill_button   (plug::Vec2d (0, 50),  50, 50, fill_released_texture,  fill_released_texture,   tool_run_fn, (void *)&main_window, (void *)&fill_tool,   PRESS_BUTTON);
    Texture_button text_button   (plug::Vec2d (50, 50), 50, 50, text_released_texture,  text_released_texture,   tool_run_fn, (void *)&main_window, (void *)&text_tool,   PRESS_BUTTON);
    
    String_button light_incr_tool_button      (plug::Vec2d (0, 0),   60, 20, "light++",     plug::Purple, plug::Purple, tool_run_fn, (void *)&main_window, (void *)&light_incr_args,        PRESS_BUTTON);
    String_button light_decr_tool_button      (plug::Vec2d (60, 0),  60, 20, "light--",     plug::Purple, plug::Purple, tool_run_fn, (void *)&main_window, (void *)&light_decr_args,        PRESS_BUTTON);
    String_button saturation_incr_tool_button (plug::Vec2d (120, 0), 60, 20, "satur++",     plug::Purple, plug::Purple, tool_run_fn, (void *)&main_window, (void *)&saturation_incr_args,   PRESS_BUTTON);
    String_button saturation_decr_tool_button (plug::Vec2d (180, 0), 60, 20, "satur--",     plug::Purple, plug::Purple, tool_run_fn, (void *)&main_window, (void *)&saturation_decr_args,   PRESS_BUTTON);
    // String_button black_white_tool_button     (plug::Vec2d (240, 0), 60, 20, "black-white", plug::Purple, plug::Purple, tool_run_fn, (void *)&main_window, (void *)&black_white_args,       PRESS_BUTTON);
    String_button loaded_filter_tool_button   (plug::Vec2d (240, 0), 60, 20, "loaded",      plug::Purple, plug::Purple, tool_run_fn, (void *)&main_window, (void *)&loaded_filter_args,     PRESS_BUTTON);
    
    // main_window.add_menu_button (&filter_tool_button);
    String_button filter_button (plug::Vec2d (50, 0), 60, 20, "Filters", plug::Purple, plug::Purple, nullptr, nullptr, nullptr);
    
    List_button filters (&filter_button);
    filters.add_button (&light_incr_tool_button);
    filters.add_button (&light_decr_tool_button);
    filters.add_button (&saturation_incr_tool_button);
    filters.add_button (&saturation_decr_tool_button);
    // filters.add_button (&black_white_tool_button);
    filters.add_button (&loaded_filter_tool_button);

    main_window.add_menu_button (&filters);
    
    button_palette.add_tool_button (&brush_button);
    button_palette.add_tool_button (&line_button);
    button_palette.add_tool_button (&circle_button);
    button_palette.add_tool_button (&rect_button);
    button_palette.add_tool_button (&fill_button);
    button_palette.add_tool_button (&text_button);
    
    button_palette.add_tool_button (&red_button);
    button_palette.add_tool_button (&blue_button);
    button_palette.add_tool_button (&green_button);
    button_palette.add_tool_button (&white_button);
    button_palette.add_tool_button (&black_button);
    button_palette.add_tool_button (&cyan_button);
    button_palette.add_tool_button (&purple_button);
    button_palette.add_tool_button (&yellow_button);
    button_palette.add_tool_button (&brown_button);
    button_palette.add_tool_button (&maroon_button);
        
    widget_manager.add_widget (&button_palette);
    

    String_button file_button (plug::Vec2d (0, 0), 50, 20, "File", plug::Purple, plug::Purple, nullptr, nullptr, nullptr);
    String_button save_button (plug::Vec2d (0, 0), 0, 20,  "Save", plug::Purple, plug::Purple, save_file, &main_window, nullptr, PRESS_BUTTON);

    List_button file_list (&file_button);
    file_list.add_button (&save_button);
    main_window.add_menu_button (&file_list);

    //Button (pos + plug::Vec2d (0, 30), 50, 20, (Button_run_fn)tool_button_act, (void *)tool_palette, (void *)tool, Color (255, 0, 0, 255))
    //this button in run func just changes current tool in tool_palette to tool; ()it's one for all tool buttons;

    while (window.isOpen ())
    {
        sf::Event event;

        if (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }

            float delta_time = clock.getElapsedTime ().asSeconds();
            widget_manager.onEvent (&event, delta_clock + delta_time);
            delta_clock = clock.getElapsedTime ().asSeconds() - delta_time;
            clock.restart ();
        }       
        else
        {
            float delta_time = clock.getElapsedTime ().asSeconds();
            widget_manager.onEvent (nullptr, delta_clock + delta_time);
            delta_clock = clock.getElapsedTime ().asSeconds() - delta_time;
            clock.restart ();
        }

        // if (status)
        {
            widget_manager.render (render_texture);
            render_texture.display ();
            // window_sprite.setTexture (render_texture.getTexture ()); //TODO
            ((sf::Sprite *)window_sprite.drawable)->setTexture (render_texture.render_texture_.getTexture ());

            window.clear ();
            window.draw (*((sf::Sprite *)(window_sprite.drawable)));
            window.display ();
            status = false;
        }
    }

    return 0;
}

bool save_file (void *widget, void *arg)
{
    assert (widget);

    Master_window *m_window = (Master_window *)widget;
    static Font text_font;
    text_font.loadFromFile (DEFAULT_FONT_FILE);

    static Text enter_request_text;
                enter_request_text.setString ("Enter filename:");
                enter_request_text.setFont (text_font);

    char buf[512] = {};
    scanf ("%s", buf);

    Window *cur_window = m_window->get_list_elem (1);
    assert (cur_window);
    CanvasView *cur_canvas = cur_window->get_canvas ();
    assert (cur_canvas);

    plug::Texture texture (cur_canvas->getTexture ());
    // bool status = texture.copyToImage ().saveToFile (buf);
    //sf::image = texture color data
    //sf image save to file
    
    return true;//status
}

plug::Filter *getFilter (const char *plugin_path)
{
    void *dll_start = dlopen (plugin_path, RTLD_NOW | RTLD_NODELETE | RTLD_LOCAL);
    
    const char *error1 = dlerror ();

    if (error1 != NULL)
    {
        fprintf (stderr, "dlerror: %s.\n", error1);
        // dlclose (dll_start);
        // return nullptr;
    } 
    else 
    {
        printf ("no error1\n");
    }

    getInterfaceFunc *plugin_func = (getInterfaceFunc *)dlsym (dll_start, "getMyFilter");
    const char *error = dlerror ();

    if (error != NULL)
    {
        fprintf (stderr, "dlerror: %s.\n", error);
        dlclose (dll_start);
        return nullptr;
    } 

    plug::Filter *plugin = (plug::Filter *)plugin_func ((size_t)plug::PluginGuid::Filter);
    
    dlclose (dll_start);
    return plugin;
}