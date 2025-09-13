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
