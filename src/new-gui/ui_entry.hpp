#ifndef UI_ENTRY_HPP
#define UI_ENTRY_HPP


#include <thread>


#include "UI/sensor_list.hpp"
#include "UI/ui.hpp"



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

void SetDefaultTheme();
void ui_main();
#endif