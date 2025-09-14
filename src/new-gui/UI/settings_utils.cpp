#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>

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
    cb_provider_ipmi           = liveSettings.provider_ipmi         ;
    sl_plot_buf_size           = liveSettings.scroll_buffer_size    ;
    sl_plot_buf_history        = liveSettings.scroll_buffer_history ;
    sl_update_interval         = liveSettings.update_interval       ;
    cb_skip_mod_load           = liveSettings.skip_module_loading   ;
    cb_emergency_cooling       = liveSettings.emergency_cooling     ;
    
    // Update atomic bool for sensor list
    sensor_list_lmsensors = liveSettings.provider_lmsensors;
    sensor_list_atasmart  = liveSettings.provider_atasmart;
    sensor_list_udisks2   = liveSettings.provider_udisks2;
    sensor_list_gtop      = liveSettings.provider_gtop;
    sensor_list_amd       = liveSettings.provider_amd;
    sensor_list_nvidia    = liveSettings.provider_nvidia;
    sensor_list_hddtemp   = liveSettings.provider_hddtemp;
    sensor_list_up_interv = liveSettings.update_interval;
    
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
