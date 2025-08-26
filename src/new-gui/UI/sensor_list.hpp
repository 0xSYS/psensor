#ifndef SENSOR_LIST_HPP
#define SENSOR_LIST_HPP

#include "imgui/imgui.h"
#include <iostream>



typedef struct
{
    std::string name;
    int current_value;
    int min;
    int max;
    ImVec4 graph_color;
    bool graph_visible;
}ui_sensor;


void create_sensor_list();
void update_sensor_list();

#endif