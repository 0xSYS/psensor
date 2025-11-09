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








#ifndef CONFIG_HPP
#define CONFIG_HPP


#include <string>



#define PSENSOR_SETTINGS_PATH "/.config/omnisensor/settings.json"



typedef struct
{
    int r;
    int g;
    int b;
    int a;
}RGBA_int;

typedef struct
{
    int window_w;
    int window_h;
    RGBA_int bg_color;
    std::string ui_theme_path;
    int graphics_platform;
    bool autosave_settings;
    bool allow_screen_saver;
    
    /* - - - - Psensor Providers - - - - */
    bool provider_gtop;
    bool provider_lmsensors;
    bool provider_udisks2;
    bool provider_atasmart;
    bool provider_hddtemp;
    bool provider_amd;
    bool provider_nvidia;
    bool provider_bcm2835;
    bool provider_ipmi;
    
    /* - - - - UI Settings - - - - */
    bool save_ui_layout;
    float ui_font_size;
    
    /* - - - - Sensor List - - - - */
    bool use_celsius_temp_unit;
    
    /* - - - - Plot Settings - - - - */
    int scroll_buffer_size;
    float scroll_buffer_history;
    int update_interval;
    
    /* - - - - Fan Controller - - - - */
    bool skip_module_loading;
    bool emergency_cooling;
    std::string cooling_preset; // Holds the filename of the preset (silent_cooling.json)
}pconfig;



pconfig readConfig();
void writeConfig(const pconfig config);
void printConfig(const pconfig config);
#endif