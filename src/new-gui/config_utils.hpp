#ifndef CONFIG_HPP
#define CONFIG_HPP


#include <string>



#define PSENSOR_SETTINGS_PATH "/.config/psensor/settings.json"


typedef struct
{
    int window_w;
    int window_h;
    std::string graphics_platform;
    
    /* - - - - Psensor Providers - - - - */
    bool provider_gtop;
    bool provider_lmsensors;
    bool provider_udisks2;
    bool provider_atasmart;
    bool provider_hddtemp;
    bool provider_amd;
    bool provider_nvidia;
    
    /* - - - - UI Settings - - - - */
    bool save_ui_layout;
    
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