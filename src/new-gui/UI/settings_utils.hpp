#ifndef SETTINGS_UTILS_HPP
#define SETTINGS_UTILS_HPP


#include "../config_utils.hpp"


inline pconfig liveSettings;

static pconfig defaultSettings =
{
    .window_w = 1020,
    .window_h = 820,
    .graphics_platform     = 0, /* Auto */
    .autosave_settings     = true,
    .provider_gtop         = true,
    .provider_lmsensors    = true,
    .provider_udisks2      = true,
    .provider_atasmart     = true,
    .provider_hddtemp      = true,
    .provider_amd          = true,
    .provider_nvidia       = true,
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