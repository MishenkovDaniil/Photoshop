#include <cassert>
#include <ctime>
#include <dlfcn.h>
#include <iostream>

#include "widget/window/master_window/master_window.h"
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
#include "widget/window/curve_window/curve_window.h"
#include "widget/window/menu/menu.h"
#include "widget/window/window.h"
#include "vector.h"

#include "standard/standard.h"
// файл с именами плагинов
// рекурсивный поиск плагинов по папке (с ограничением)

static const char *FILTER_PATHS[]   = 
    {
        "/home/daniil/programming/code/C++_projects/photoshop/src/Plugins/TimAFilter/test_filter.so",
        "/home/daniil/programming/code/C++_projects/photoshop/src/Plugins/DanyaKFilter/ContrastFilter.so",
        "/home/daniil/programming/code/C++_projects/photoshop/src/Plugins/saturation_filter/saturFilter.so",
        "/home/daniil/programming/code/C++_projects/photoshop/src/Plugins/PosterizeFilter/PosterizeFilter.so",
        "/home/daniil/programming/code/C++_projects/photoshop/src/Plugins/BlackWhiteFilter/BlackWhiteFilter.so"
    };
static const char *TOOL_PATHS[]     = 
    {
        "/home/daniil/programming/code/C++_projects/photoshop/src/Plugins/SelectionTool/SelectionTool.so"
    };

static const char *LOAD_PLUGIN_FUNC = "loadPlugin";

static const char *line_img   = "resources/imgs/line.png";
static const char *fill_img   = "resources/imgs/fill.png";
static const char *brush_img  = "resources/imgs/brush.png";
static const char *circle_img = "resources/imgs/circle.png";
static const char *rect_img   = "resources/imgs/rect.png";
static const char *text_img   = "resources/imgs/text.png";
// static const char *TOOL_IMGS[] = {line_img, fill_img, brush_img, circle_img, rect_img, text_img};

static const int FULL_SCREEN_WIDTH          = 1920;
static const int FULL_SCREEN_HEIGHT         = 1080;
static const int LIGHT_DELTA_CHANGE         = 5;
static const int SATURATION_DELTA_CHANGE    = 5;

struct ButtonImg 
{
    Sprite *pressed_ = nullptr;
    Sprite *released_ = nullptr;

    ButtonImg (Sprite *pressed, Sprite *released) : pressed_ (pressed), released_ (released) {};
    ~ButtonImg () = default;
};

typedef plug::Plugin *(loadPluginFunc) ();

bool fill_filters (M_vector<plug::Filter *> &filters, List_button &filter_list, Master_window &m_window, Tool_palette &palette);
void clean_filters (M_vector<plug::Filter *> &filters, List_button &filter_list);
bool fill_tools (M_vector<plug::Tool *> &tools, M_vector<ButtonImg *> &button_imgs, Tool_palette &palette);
bool fill_tool_buttons (const M_vector<plug::Tool *> &tools, const M_vector<ButtonImg *> &button_imgs, Button_palette &button_palette, Master_window &main_window);
bool fill_color_buttons (M_vector<plug::Color> &colors, Button_palette &button_palette, Master_window &main_window);

plug::Filter *getFilter (const char *plugin_path);
plug::Tool   *getTool   (const char *plugin_path);
bool is_dl_error ();

bool save_file (void *widget, void *arg);
bool curves_func (void *widget, void *arg);


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

    Window *child_window    = new Window (600, 600, plug::Vec2d (200, 200), "window_1");
    Window *child_window_2  = new Window (600, 600, plug::Vec2d (600, 600), "window_2");
    main_window.add_window (child_window);
    main_window.add_window (child_window_2);

    Sprite window_sprite;
    bool status = true;

    sf::Clock clock;
    float delta_clock = 0;

    time_t time_;
    struct tm *time_info;
    time (&time_);
    time_info = localtime (&time_);

    clock_button.change_time (time_info->tm_sec, time_info->tm_min, time_info->tm_hour);
    
    M_vector<plug::Tool *> tools (nullptr);
    M_vector<ButtonImg *> button_imgs (nullptr);
    M_vector<plug::Color> colors (plug::Color (0, 0, 0));

    colors.add (plug::Red);
    colors.add (plug::Blue);
    colors.add (plug::Green);
    colors.add (plug::White);
    colors.add (plug::Black);
    colors.add (plug::Cyan);
    colors.add (plug::Purple);
    colors.add (plug::Yellow);
    colors.add (plug::Brown);
    colors.add (plug::Maroon);
    
    Button_palette button_palette (plug::Vec2d (1650, 50), 200, 200, &palette); 
    
    fill_tools (tools, button_imgs, palette);
    fill_tool_buttons (tools, button_imgs, button_palette, main_window);
    fill_color_buttons (colors, button_palette, main_window);

    M_vector<plug::Filter *> filters (nullptr);
    String_button filter_button (plug::Vec2d (50, 0), 60, 20, "Filters", plug::Purple, plug::Purple, nullptr, nullptr, nullptr);    
    List_button filter_buttons_list (&filter_button);
    fill_filters (filters, filter_buttons_list, main_window, palette);

    widget_manager.add_widget (&button_palette);
    
    String_button file_button (plug::Vec2d (0, 0), 50, 20, "File", plug::Purple, plug::Purple, nullptr, nullptr, nullptr);
    String_button save_button (plug::Vec2d (0, 0), 0, 20,  "Save", plug::Purple, plug::Purple, save_file, &main_window, nullptr, PRESS_BUTTON);

    List_button file_list (&file_button);
    file_list.add_button (&save_button);
    main_window.add_menu_button (&file_list);
    main_window.add_menu_button (&filter_buttons_list);

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
            widget_manager.draw (render_texture);
            render_texture.display ();
            // window_sprite.setTexture (render_texture.getTexture ()); //TODO
            ((sf::Sprite *)window_sprite.drawable)->setTexture (render_texture.render_texture_.getTexture ());

            window.clear ();
            window.draw (*((sf::Sprite *)(window_sprite.drawable)));
            window.display ();
            status = false;
        }
    }
    
    clean_filters (filters, filter_buttons_list);

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

bool curves_func (void *widget, void *arg)
{
    Master_window *m_window = (Master_window *)widget;
    std::pair<void *, void *> *pair = (std::pair<void *, void *> *)arg;
    plug::Canvas *active_canvas = (plug::Canvas *)pair->first;
    Curve_filter *curve_filter = (Curve_filter *)pair->second;

    Tool_palette *curve_palette = new Tool_palette;
    CurveTool *curve_tool = new CurveTool (curve_filter, active_canvas);
    curve_palette->add_tool (curve_tool);

    CurveWindow *curve_window = new CurveWindow (562, 592, m_window->getLayoutBox ().getSize () / 2, "Curves", curve_palette, active_canvas, curve_filter, curve_tool);
    m_window->add_window (curve_window);

    CanvasView *view = curve_window->get_canvas ();
    assert (view);
    Canvas *canvas = view->getCanvas ();
    assert (canvas);

    curve_tool->setActiveCanvas (*canvas);
    curve_tool->setColorPalette (curve_palette->color_palette_);
    plug::ControlState state;
    state.state = plug::State::Pressed;
    plug::Vec2d pos;
    curve_tool->onMainButton (state, pos);
    curve_palette->cur_tool_ = curve_tool;

    return true;
}

bool fill_filters (M_vector<plug::Filter *> &filters, List_button &filter_list, Master_window &main_window, Tool_palette &palette)
{
    String_button *curves_button = new String_button (plug::Vec2d (110, 0), 60, 20, "Curves", plug::Purple, plug::Purple, curves_func, &main_window, nullptr);
    
    plug::Filter *my_filter = new Light_filter (LIGHT_DELTA_CHANGE);
    filters.add (my_filter);
                  my_filter = new Light_filter (-LIGHT_DELTA_CHANGE);
    filters.add (my_filter);
                  my_filter = new Curve_filter (curves_button);
    filters.add (my_filter);
                  my_filter = new White_black_filter;
    filters.add (my_filter);

    const size_t Load_filter_num = sizeof (FILTER_PATHS) / sizeof (FILTER_PATHS[0]);
    for (size_t load_filter_idx = 0; load_filter_idx < Load_filter_num; ++load_filter_idx)
    {
        plug::Filter *loaded_filter = getFilter (FILTER_PATHS[load_filter_idx]);
        if (loaded_filter)
        {
            filters.add (loaded_filter);
        }  
        else 
        {
            fprintf (stderr, "Error: failed to load filter from %s.\n", FILTER_PATHS[load_filter_idx]);
            return false;
        } 
    }

    const size_t Filter_num = filters.get_size ();
    plug::Vec2d pos;
    plug::Vec2d button_size (60, 20);
    
    for (size_t filter_idx = 0; filter_idx < Filter_num; ++filter_idx)
    {
        filters[filter_idx]->addReference ();
        Filter_tool * cur_filter_tool  = new Filter_tool (filters[filter_idx]);
        assert (cur_filter_tool);
        Pair *cur_args = new Pair ((void *)&palette, cur_filter_tool);
        assert (cur_args);
        
        String_button *cur_button = new String_button (pos, button_size.x, button_size.y, filters[filter_idx]->getPluginData ()->getName (), 
                                    plug::Purple, plug::Purple, tool_run_fn, (void *)&main_window, cur_args, PRESS_BUTTON);
        assert (cur_button);
        
        filter_list.add_button (cur_button);
        
        pos += button_size;
    }

    return true;
}

bool fill_tools (M_vector<plug::Tool *> &tools, M_vector<ButtonImg *> &button_imgs, Tool_palette &palette)
{
    Sprite *brush_sprite_pressed    = new Sprite (brush_img);
    Sprite *brush_sprite_released   = new Sprite (brush_img);
    Sprite *line_sprite_pressed     = new Sprite (line_img);
    Sprite *line_sprite_released    = new Sprite (line_img);
    Sprite *circle_sprite_pressed   = new Sprite (circle_img);
    Sprite *circle_sprite_released  = new Sprite (circle_img);
    Sprite *rect_sprite_pressed     = new Sprite (rect_img);
    Sprite *rect_sprite_released    = new Sprite (rect_img);
    Sprite *fill_sprite_pressed     = new Sprite (fill_img);
    Sprite *fill_sprite_released    = new Sprite (fill_img);
    Sprite *text_sprite_pressed     = new Sprite (text_img);
    Sprite *text_sprite_released    = new Sprite (text_img);

    tools.add (new class Brush);
    tools.add (new Line);
    tools.add (new Circle_shape);
    tools.add (new Rect_shape);
    tools.add (new Fill);
    tools.add (new Text_tool);

    button_imgs.add (new ButtonImg (brush_sprite_pressed, brush_sprite_released));
    button_imgs.add (new ButtonImg (line_sprite_pressed, line_sprite_released));
    button_imgs.add (new ButtonImg (circle_sprite_pressed, circle_sprite_released));
    button_imgs.add (new ButtonImg (rect_sprite_pressed, rect_sprite_released));
    button_imgs.add (new ButtonImg (fill_sprite_pressed, fill_sprite_released));
    button_imgs.add (new ButtonImg (text_sprite_pressed, text_sprite_released));

    size_t load_tool_num = sizeof (TOOL_PATHS) / sizeof (TOOL_PATHS[0]);
    for (size_t load_tool_idx = 0; load_tool_idx < load_tool_num; ++load_tool_idx)
    {
        plug::Tool *loaded_tool = getTool (TOOL_PATHS[load_tool_idx]);
        if (!loaded_tool)
            return false;
        tools.add (loaded_tool);
    }

    size_t tool_num = tools.get_size ();
    for (size_t tool_idx = 0; tool_idx < tool_num; ++tool_idx)
    {
        palette.add_tool (tools[tool_idx]);
    }

    return true;
}

bool fill_tool_buttons (const M_vector<plug::Tool *> &tools, const M_vector<ButtonImg *> &button_imgs, Button_palette &button_palette, Master_window &main_window)
{
    plug::Vec2d tool_button_size (50, 50);
    size_t tools_in_row = button_palette.getLayoutBox ().getSize ().y / tool_button_size.y;
    plug::Vec2d tool_button_pos;

    for (size_t i = 0; i < tools.get_size (); ++i)
    {
        if (button_imgs[i] && button_imgs[i]->pressed_ && button_imgs[i]->released_)
        {
            button_palette.add_tool_button (new Texture_button (tool_button_pos, tool_button_size.x, tool_button_size.y, 
                                                  *(button_imgs[i]->pressed_),  *(button_imgs[i]->released_),  
                                                  tool_run_fn, (void *)&main_window, (void *)tools[i],  PRESS_BUTTON));
        }
        else if (tools[i]->getPluginData () && tools[i]->getPluginData ()->getName ())
        {
            button_palette.add_tool_button (new String_button (tool_button_pos, tool_button_size.x, tool_button_size.y, 
                                                  tools[i]->getPluginData ()->getName (),  plug::Transparent, plug::Transparent, 
                                                  tool_run_fn, (void *)&main_window, (void *)tools[i],  PRESS_BUTTON));
        }
        else 
        {
            return false;
        }
        tool_button_pos = (i + 1) % tools_in_row ? tool_button_pos + plug::Vec2d (tool_button_size.x, 0) : plug::Vec2d (0, tool_button_pos.y + tool_button_size.y);
    }

    return true;
}

bool fill_color_buttons (M_vector<plug::Color> &colors, Button_palette &button_palette, Master_window &main_window)
{
    plug::Vec2d color_button_size (20, 20);
    size_t colors_in_row = 5;
    plug::Vec2d color_button_pos (0, 160);
    size_t color_num = colors.get_size ();

    for (size_t idx = 0; idx < color_num; ++idx)
    {
        button_palette.add_tool_button (new Button (color_button_pos, color_button_size.x, color_button_size.y, color_button_run_fn, &main_window, &(colors[idx]), colors[idx], PRESS_BUTTON));
        color_button_pos = (idx + 1) % colors_in_row ? color_button_pos + plug::Vec2d (color_button_size.x, 0) : plug::Vec2d (0, color_button_pos.y + color_button_size.y);
    }

    return true;
}

void clean_filters (M_vector<plug::Filter *> &filters, List_button &filter_list)
{
    const size_t Filter_num = filters.get_size ();
    
    for (size_t filter_idx = 0; filter_idx < Filter_num; ++filter_idx)
    {
        Button *button = filter_list.buttons_[filter_idx];
        Pair *args = (Pair *)(button->get_arg ());
        Filter_tool *filter_tool  = (Filter_tool *)(args->arg_2);

        delete filter_tool;
        delete args;
        delete button;
        filters[filter_idx]->release ();
    }
    for (size_t filter_idx = 0; filter_idx < Filter_num; ++filter_idx)
    {
        filters.pop ();
        filter_list.buttons_.pop ();
    }
}

plug::Filter *getFilter (const char *plugin_path)
{
    assert (plugin_path);

    void *dll_start = dlopen (plugin_path, RTLD_NOW | RTLD_NODELETE | RTLD_LOCAL);
    
    if (is_dl_error ())
    {
        dlclose (dll_start);
        return nullptr;
    } 

    loadPluginFunc *plugin_func = (loadPluginFunc *)dlsym (dll_start, LOAD_PLUGIN_FUNC);

    if (is_dl_error ())
    {
        dlclose (dll_start);
        return nullptr;
    } 

    plug::Filter *plugin = (plug::Filter *)plugin_func ();
    
    dlclose (dll_start);
    return plugin;
}

plug::Tool *getTool (const char *plugin_path)
{
    void *dll_start = dlopen (plugin_path, RTLD_NOW | RTLD_NODELETE | RTLD_LOCAL);
    
    if (is_dl_error ())
    {
        dlclose (dll_start);
        return nullptr;
    } 

    loadPluginFunc *plugin_func = (loadPluginFunc *)dlsym (dll_start, LOAD_PLUGIN_FUNC);

    if (is_dl_error ())
    {
        dlclose (dll_start);
        return nullptr;
    } 

    plug::Tool *plugin = (plug::Tool *)plugin_func ();
    
    dlclose (dll_start);
    return plugin;
}

bool is_dl_error ()
{
    const char *error = dlerror ();

    if (error != NULL)
    {
        fprintf (stderr, "dlerror: %s.\n", error);
        return true;
    } 
    return false;
}
