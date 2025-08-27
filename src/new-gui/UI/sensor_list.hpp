#ifndef SENSOR_LIST_HPP
#define SENSOR_LIST_HPP

#include "imgui/imgui.h"
#include <iostream>
#include <vector>
#include <mutex>



inline std::mutex sensor_list_mutex;
inline bool sensor_list_created = false;
inline int sensor_count = -1;

const std::vector <ImVec4> graph_colors=
{
    {0,   255, 243, 255},
    {0,   130, 255, 255},
    {231, 227, 44,  255},
    {255, 0,   254, 255},
    {0,   25,  31,  255},
    {255, 112, 0,   255},
    {180, 0,   255, 255},
    {255, 0,   0,   255},
    {191, 185, 27,  255},
    {255, 187, 53,  255},
    {194, 76,  109, 255},
    {240, 115, 255, 255},
    {26,  163, 37,  255},
    {255, 75,  0,   255},
    {213, 227, 100, 255},
    {0,   255, 158, 255},
    {20,  89,  14,  255},
    {145, 20,  122, 255},
};

struct ui_sensor
{
    std::string name;
    double current_value;
    double min;
    double max;
    
    ImVec4 graph_color;
    bool graph_visible;
    ui_sensor(const std::string& n, double cv, double mn, double mx, const ImVec4& gc, bool gv)
        : name(n), current_value(cv), min(mn), max(mx), graph_color(gc), graph_visible(gv) {}
};

void create_sensor_list();
void update_sensor_list(std::vector<ui_sensor>& sl);

#endif