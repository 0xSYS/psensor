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








#ifndef SETTINGS_UTILS_HPP
#define SETTINGS_UTILS_HPP


#include "../config_utils.hpp"


inline pconfig liveSettings;
inline pconfig initialSettings;

static pconfig defaultSettings =
{
    .window_w = 1020,
    .window_h = 820,
    .bg_color = { .r = 87,  .g = 87, .b = 87, .a = 255},
    .ui_theme_path         = "",
    .graphics_platform     = 0, /* Auto */
    .autosave_settings     = true,
    .allow_screen_saver    = true,
    .provider_gtop         = true,
    .provider_lmsensors    = true,
    .provider_udisks2      = true,
    .provider_atasmart     = true,
    .provider_hddtemp      = true,
    //.provider_amd          = true,
    .provider_roc_smi      = true,
    .provider_nvidia       = true,
    .provider_bcm2835      = true,
    .provider_ipmi         = false,
    .save_ui_layout        = true,
    .ui_font_size          = 25.0f,
    .use_celsius_temp_unit = true,
    .scroll_buffer_size    = 1200,
    .scroll_buffer_history = 30.0f,
    .update_interval       = 1000,
    .skip_module_loading   = false,
    .emergency_cooling     = false,
};



void SyncSettings();
void SaveSettings();

#endif