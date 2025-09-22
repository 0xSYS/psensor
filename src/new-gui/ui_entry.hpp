#ifndef UI_ENTRY_HPP
#define UI_ENTRY_HPP


#include <thread>
#include <vector>


#include "UI/sensor_list.hpp"
//#include "UI/ui.hpp"



static const char * sdl_graphics_platforms[] =
{
    "vulkan",
    "opengl",
    "opengles2",
    "software",
    nullptr
};


inline std::thread sensor_update_thr;
inline size_t ui_themes_count = 0;
inline std::vector<std::string> color_themes;
inline std::vector<ui_sensor> initial_sensor_properties;
void RefreshColorThemesFiles();

void SetDefaultTheme();
void ui_main();
#endif