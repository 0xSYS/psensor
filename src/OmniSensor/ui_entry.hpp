/*
 * Copyright (C) 2025 kiptunor
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */








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
inline int initial_theme_index = 0;
void RefreshColorThemesFiles();

void SetDefaultTheme();
void ui_main();
#endif