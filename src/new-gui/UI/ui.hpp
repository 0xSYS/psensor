#ifndef UI_HPP
#define UI_HPP

#include <vector>
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

void RenderUI();
#endif