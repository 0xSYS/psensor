#ifndef UI_HPP
#define UI_HPP

#include <vector>
#include <implot/implot.h>
#include "plot.hpp"
#include "sensor_list.hpp"

inline bool loop_exit = false;
inline bool sensor_list = true;
inline bool fan_controller_open = false;
inline bool sensor_settings = false;
inline bool preferences = false;
inline bool about_window = false;
inline bool no_root_open = false;
inline bool no_fc_available_mod = false;
inline std::vector<ui_sensor> sensor;
inline std::vector<bool> sensor_graph_enabled;
inline std::vector<ImVec4> sensor_graph_color;
inline std::vector<ScrollingBuffer> sensor_plots;


inline int selected_graphics_platform = 0;
inline const char * graphics_plaforms[] =
{
    "Auto",
    "Vulkan",
    "OpenGL",
    "OpenGLES2"
};

inline bool save_ui_layouts;
inline bool autosave_settings;
inline bool temp_autosave_settings;

inline bool cb_provider_lmsensors;
inline bool cb_provider_udisks2  ;
inline bool cb_provider_hddtemp  ;
inline bool cb_provier_arasmart  ;
inline bool cb_provider_gtop     ;
inline bool cb_provider_amd      ;
inline bool cb_provider_nvidia   ;
inline bool cb_provider_ipmi     ;

inline bool cb_use_celsiustemp;

inline int sl_plot_buf_size;
inline float sl_plot_buf_history;
inline int sl_update_interval;

inline bool cb_skip_mod_load;
inline bool cb_emergency_cooling;




void RenderUI();
#endif