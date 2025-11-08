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








//#include <iostream>
//#include <string>
#include <sstream>
#include <filesystem>

#include <imgui/imgui.h>

#include <log_c/log.h>


#include "settings_utils.hpp"
#include "ui.hpp"
#include "../config_utils.hpp"
#include "../utils.hpp"






void SyncSettings()
{
    std::ostringstream temp_path;
    temp_path << Utils::get_home_dir() << PSENSOR_SETTINGS_PATH;

    if(std::filesystem::exists(temp_path.str()))
        liveSettings = readConfig();
    else
        liveSettings = defaultSettings;
    
    
    initialSettings = liveSettings;

    // Here's the actual settings syncronization to the UI

    cb_use_celsiustemp         = liveSettings.use_celsius_temp_unit ;
    autosave_settings          = liveSettings.autosave_settings     ;
    cb_allow_screen_saver      = liveSettings.allow_screen_saver    ;
    temp_autosave_settings     = liveSettings.autosave_settings     ;
    save_ui_layouts            = liveSettings.save_ui_layout        ;
    cb_provider_lmsensors      = liveSettings.provider_lmsensors    ;
    cb_provider_udisks2        = liveSettings.provider_udisks2      ;
    cb_provider_hddtemp        = liveSettings.provider_hddtemp      ;
    cb_provider_atasmart       = liveSettings.provider_atasmart     ;
    cb_provider_gtop           = liveSettings.provider_gtop         ;
    cb_provider_amd            = liveSettings.provider_amd          ;
    cb_provider_nvidia         = liveSettings.provider_nvidia       ;
    cb_provider_bcm2835        = liveSettings.provider_bcm2835      ;
    cb_provider_ipmi           = liveSettings.provider_ipmi         ;
    sl_plot_buf_size           = liveSettings.scroll_buffer_size    ;
    sl_plot_buf_history        = liveSettings.scroll_buffer_history ;
    sl_update_interval         = liveSettings.update_interval       ;
    cb_skip_mod_load           = liveSettings.skip_module_loading   ;
    cb_emergency_cooling       = liveSettings.emergency_cooling     ;
    //selected_ui_theme          = liveSettings.color_theme_index     ; // Not available anymore, theme handling is now better and more stable
    
    // Update atomic bool for sensor list
    sensor_list_lmsensors = liveSettings.provider_lmsensors;
    sensor_list_atasmart  = liveSettings.provider_atasmart;
    sensor_list_udisks2   = liveSettings.provider_udisks2;
    sensor_list_gtop      = liveSettings.provider_gtop;
    sensor_list_amd       = liveSettings.provider_amd;
    sensor_list_nvidia    = liveSettings.provider_nvidia;
    sensor_list_hddtemp   = liveSettings.provider_hddtemp;
    sensor_list_up_interv = liveSettings.update_interval;
    
    ImVec4 temp_color;
    temp_color.x = liveSettings.bg_color.r;
    temp_color.y = liveSettings.bg_color.g;
    temp_color.z = liveSettings.bg_color.b;
    temp_color.w = liveSettings.bg_color.a;
    
    clear_color = ImVec4_RGBtoFloat(temp_color);
    
    if(liveSettings.graphics_platform > 3)
    {
        log_error("Invalid graphics platform index !!! Falling back to auto");
        liveSettings.graphics_platform = 0;
    }
    else
        selected_graphics_platform = liveSettings.graphics_platform     ;

}

void SaveSettings()
{
    if(liveSettings.autosave_settings)
    {
        writeConfig(liveSettings);
    }
    else
    {
        log_info("No auto save");
        return;
    }
}
