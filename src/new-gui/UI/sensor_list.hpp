#ifndef SENSOR_LIST_HPP
#define SENSOR_LIST_HPP

#include <imgui/imgui.h>
#include <iostream>
#include <vector>
#include <mutex>
#include <atomic>



inline std::mutex sensor_list_mutex;
inline std::mutex sensors_mutex;
inline bool sensor_list_created = false;
inline std::atomic<bool> keep_sensor_update{true};
inline std::atomic<bool> sensor_list_lmsensors;
inline std::atomic<bool> sensor_list_atasmart;
inline std::atomic<bool> sensor_list_udisks2;
inline std::atomic<bool> sensor_list_gtop;
inline std::atomic<bool> sensor_list_nvidia;
inline std::atomic<bool> sensor_list_bcm2835;
inline std::atomic<bool> sensor_list_amd;
inline std::atomic<bool> sensor_list_hddtemp;
inline std::atomic<int> sensor_list_up_interv;
inline int sensor_count = -1;
inline struct psensor **sensors = nullptr;
inline struct psensor* temp_s;
inline float plot_update_interval = 0.0f;
inline std::vector<std::string> sensor_names;

const std::vector <ImVec4> graph_colors=
{
    {0,   255, 243, 255},
    {0,   130, 255, 255},
    {231, 227, 44,  255},
    {255, 0,   254, 255},
    {0,   255, 31,  255},
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
    {255, 255, 255, 255},
    {33,  140,  20, 255},
    {100, 250, 255, 255},
    {90,  80,  100, 255},
};

struct ui_sensor
{
    std::string name;
    double current_value;
    double min;
    double max;
    int sensor_type;
    std::string chip;
    std::string sensor_id;
    
    ImVec4 graph_color;
    bool graph_visible;
    ui_sensor(const std::string& n, double cv, double mn, double mx, int st, const std::string& c, const std::string& i, const ImVec4& gc, bool gv)
        : name(n), current_value(cv), min(mn), max(mx), sensor_type(st), chip(c), sensor_id(i), graph_color(gc), graph_visible(gv) {}
};



void create_sensor_list();
void update_sensor_list(std::vector<ui_sensor>& sl);

#endif