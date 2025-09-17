#ifndef CONFIG_HPP
#define CONFIG_HPP


#include <string>



#define PSENSOR_SETTINGS_PATH "/.config/psensor/settings.json"



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