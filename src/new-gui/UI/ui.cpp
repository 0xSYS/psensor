// C++ don't mangle my C functions !!! >:(
extern "C"
{
    #include <psensor/psensor.h>
}

#include <vector>
#include <mutex>
#include <map>
#include <future>
#include <chrono>


#include <log_c/log.h>

#include <imgui/imgui.h>


#include "ui.hpp"
#include "sensor_list.hpp"

#include "../utils.hpp"










psensor_fan *PCFans = {NULL};
std::map<int, std::future<void>> fanTestFutures;
std::once_flag pwm_flag;
inline std::vector<bool> is_fan_test_running;
inline std::vector<int> fan_pwm;
inline std::vector<int> prev_fan_pwm;
inline std::vector<bool> pwm_set_exe;

static int selected_graphics_platform = 0;
static const char * graphics_plaforms[] =
{
    "Auto",
    "Vulkan",
    "OpenGL",
    "OpenGLES",
    "OpenGLES2"
};

static bool save_ui_layouts = true;
static bool autosave_settings = true;

static bool cb_provider_lmsensors = true;
static bool cb_provider_udisks2   = true;
static bool cb_provider_hddtemp   = true;
static bool cb_provier_arasmart   = true;
static bool cb_provider_gtop      = true;
static bool cb_provider_amd       = true;
static bool cb_provider_nvidia    = true;

static bool cb_use_celsiustemp = true;

static int sl_plot_buf_size = 12000;
static float sl_plot_buf_history = 30.0f;
static int sl_update_interval = 1000;

static bool cb_skip_mod_load = false;
static bool cb_emergency_cooling = true;





// MARK: Internal funcs

void EnableFanPwmOnce()
{
    std::call_once(pwm_flag, []()
    {
        PCFans = psensor_detectFans();
        
        if(!PCFans || PCFans->fanInputCount <= 0)
        {
            log_error("Failed to enable fan PWM");
            return;
        }
        for(int i = 0; i < PCFans->fanInputCount; i++)
        {
            psensor_enable_fan_pwm(PCFans->pwmEnableFiles[i], 1);
        }
        
        fan_pwm.resize(PCFans->fanInputCount, 0);
        prev_fan_pwm.resize(PCFans->fanInputCount, 0);
        pwm_set_exe.resize(PCFans->fanInputCount, false);
        is_fan_test_running.resize(PCFans->fanInputCount, false);
    });
}

void SetFanPwmOnce(int index)
{
    // Ewww That's ugly
    // I hate to say but it f works ;(
    if(!pwm_set_exe[index])
    {
        psensor_fan_set_pwm(PCFans->pwmFiles[index], prev_fan_pwm[index]);
        fan_pwm[index] = prev_fan_pwm[index]; // sync UI with restored value
        pwm_set_exe[index] = true;
    }
}

// MARK: Windows

void RenderFanControllerWindow()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowSizeConstraints(ImVec2(700, 380), ImVec2(FLT_MAX, FLT_MAX));
    
    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
    
    ImGui::Begin("Fan Controller", &fan_controller_open);
    
    
    if(ImGui::BeginTable("table_item_width", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY))
    {
        ImGui::TableSetupColumn("Fan Index");
        ImGui::TableSetupColumn("Speed (PWM)");
        ImGui::TableSetupColumn("Speed Test");
        ImGui::TableHeadersRow();
        
        for(int row = 0; row < PCFans->fanInputCount; row++)
        {
            
            ImGui::TableNextRow();
            if(row == 0)
            {
                ImGui::TableSetColumnIndex(0);
                ImGui::PushItemWidth(TEXT_BASE_WIDTH * 3.0f);
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::TableSetColumnIndex(2);
                ImGui::PushItemWidth(-FLT_MIN);
            }
            
            static int dummy_f = 0;
            ImGui::PushID(row);
            
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Fan %d", row);
            ImGui::TableSetColumnIndex(1);
            ImGui::BeginDisabled(is_fan_test_running[row]);
            
            fan_pwm[row] = psensor_get_last_pwm(PCFans->pwmFiles[row]);
            
            if(ImGui::SliderInt("##SpeedCtrl", &fan_pwm[row], 0, 255))
            {
                psensor_fan_set_pwm(PCFans->pwmFiles[row], fan_pwm[row]);
            }
            
            ImGui::TableSetColumnIndex(2);
            if(ImGui::Button("Test"))
            {
                prev_fan_pwm[row] = fan_pwm[row];
                fanTestFutures[row] = std::async(std::launch::async, [row]()
                {
                    psensor_test_fan(PCFans->pwmFiles[row]);
                });
            }
            
            ImGui::EndDisabled();
            
            if(auto it = fanTestFutures.find(row); it != fanTestFutures.end())
            {
                auto status = it->second.wait_for(std::chrono::milliseconds(0));
                if(status == std::future_status::ready)
                {
                    is_fan_test_running[row] = false;
                    SetFanPwmOnce(row);
                    pwm_set_exe[row] = false; // Reset my brain here
                    fanTestFutures.erase(it);
                }
                else if(status == std::future_status::timeout)
                {
                    is_fan_test_running[row] = true;
                }
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    
    ImGui::End();
}

void RenderAboutWindow()
{
    if(about_window)
    {
        ImGuiIO& io = ImGui::GetIO();
        
        ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
        ImGui::SetNextWindowPos(center, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSizeConstraints(ImVec2(500, 500), ImVec2(FLT_MAX, FLT_MAX));
        ImGui::Begin("About", &about_window, 0);
        ImVec2 avail_size = ImGui::GetContentRegionAvail();
        ImGui::BeginChild("ScrollRegion", avail_size, true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::Text("Psensor");
        ImGui::Text("A fork of the original Psensor project created by jeanfi@gmail.com");
        ImGui::Text("This fork aims to provide a new look and feel for the desktop application as well as new features.");
        ImGui::Text("");
        ImGui::Text("Source code available on ");
        ImGui::SameLine();
        ImGui::TextLinkOpenURL("GitHub", "https://github.com/0xSYS/psensor");
        ImGui::Text("Psensor %s", VERSION);
        ImGui::Text("Copyright (C) 2010-2025");
        ImGui::Text("jeanfi@gmail.com");
        ImGui::Text("xsys061@gmail.com");
        ImGui::Text("");
        ImGui::Text("License: GPLv2");
        //ImGui::Text("Website: https://psensor.org"); // Not yet
        //ImGui::End();
        ImGui::EndChild();
        ImGui::End();
    }
}

void RenderSensorSettings()
{
    ImGui::SetNextWindowSizeConstraints(ImVec2(500, 500), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin("Sensor Settings", &sensor_settings);
    ImGui::End();
}

void RenderPreferences()
{
    ImGui::SetNextWindowSizeConstraints(ImVec2(500, 500), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin("Preferences", &preferences);
    if(ImGui::BeginTabBar("##Settings", 0))
    {
        if(ImGui::BeginTabItem("General"))
        {
            ImGui::Text("* Graphics Platform: ");
            ImGui::SameLine();
            if (ImGui::BeginCombo("##gplat", graphics_plaforms[selected_graphics_platform], ImGuiComboFlags_WidthFitPreview))
            {
                for (int n = 0; n < IM_ARRAYSIZE(graphics_plaforms); n++)
                {
                    const bool is_selected = (selected_graphics_platform == n);
                    if (ImGui::Selectable(graphics_plaforms[n], is_selected))
                        selected_graphics_platform = n;
    
                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            ImGui::TextDisabled("(?)");
            if (ImGui::BeginItemTooltip())
            {
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                ImGui::Text("Set a graphics platform\nAutomatic - tries the available graphics platforms and choses the one that works");
                ImGui::Text("GUI Rendering can be slow when using the software renderer which can be applied if the rest of the graphics platforms are not available");
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }
            ImGui::Checkbox("Save UI layouts", &save_ui_layouts);
            ImGui::Checkbox("Autosave settings", &autosave_settings);
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Providers"))
        {
            ImGui::Checkbox("lm_sensors", &cb_provider_lmsensors);
            ImGui::Checkbox("udisks2",    &cb_provider_udisks2  );
            ImGui::Checkbox("hddtemp",    &cb_provider_hddtemp  );
            ImGui::Checkbox("atasmart",   &cb_provier_arasmart  );
            ImGui::Checkbox("gtop",       &cb_provider_gtop     );
            ImGui::Checkbox("amd",        &cb_provider_amd      );
            ImGui::Checkbox("nvidia",     &cb_provider_nvidia   );
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Sensor List"))
        {
            ImGui::Checkbox("Use Celsius Temperature Unit", &cb_use_celsiustemp);
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Plot settings"))
        {
            if(ImGui::SliderInt("Plot Buffer Size", &sl_plot_buf_size, 1000, 50000))
            {
                log_info("Temp...");
            }
            if(ImGui::SliderFloat("Plot History", &sl_plot_buf_history, 10.0f, 60.0f))
            {
                log_info("Temp...");
            }
            if(ImGui::SliderInt("Update Interval", &sl_update_interval, 500, 5000))
            {
                log_info("up interv...");
            }
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Fan Controller"))
        {
            ImGui::Checkbox("Skip kernel module loading", &cb_skip_mod_load);
            ImGui::Checkbox("Emergency Cooling", &cb_emergency_cooling);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void RenderSensorList()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowSizeConstraints(ImVec2(700, 380), ImVec2(FLT_MAX, FLT_MAX));
    
    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
    
    ImGui::SetNextWindowSizeConstraints(ImVec2(500, 170), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin("##Sensor List", NULL, ImGuiWindowFlags_NoCollapse);
    
    if(ImGui::BeginTable("sensor_table", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY))
    {
        ImGui::TableSetupColumn("Plot Display");
        ImGui::TableSetupColumn("Sensor");
        ImGui::TableSetupColumn("Current Value");
        ImGui::TableSetupColumn("Min");
        ImGui::TableSetupColumn("Max");
        ImGui::TableSetupColumn("Color");
        ImGui::TableHeadersRow();
        
        std::lock_guard<std::mutex> lock(sensor_list_mutex);
        for(int row = 0; row < sensor_count; row++)
        {
            auto& s = sensor[row];
            ImGui::TableNextRow();
            if(row == 0)
            {
                ImGui::TableSetColumnIndex(0);
                ImGui::PushItemWidth(TEXT_BASE_WIDTH * 3.0f);
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::TableSetColumnIndex(2);
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::TableSetColumnIndex(3);
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::TableSetColumnIndex(4);
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::TableSetColumnIndex(5);
                ImGui::PushItemWidth(-FLT_MIN);
            }
            
            ImGui::PushID(row);
            
            ImGui::TableSetColumnIndex(0);
            bool b = sensor_graph_enabled[row] != false;
            if(ImGui::Checkbox("##Graph_Show", &b))
                sensor_graph_enabled[row] = b ? true : false;
            
            ImGui::PopID();
            
            ImGui::TableSetColumnIndex(1);
            
            ImGui::Text("%s", s.name.c_str());
            
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", psensor_value_to_str(s.sensor_type, s.current_value, 1));
            
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%s", psensor_value_to_str(s.sensor_type, s.min, 1));
            
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%s", psensor_value_to_str(s.sensor_type, s.max, 1));
            
            ImGui::PushID(row);
            ImGui::TableSetColumnIndex(5);
            ImGui::ColorEdit3("##Graph_Color", &sensor_graph_color[row].x, ImGuiColorEditFlags_NoInputs);
            
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    ImGui::End();
}

void RenderSensorPlot()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowSizeConstraints(ImVec2(700, 380), ImVec2(FLT_MAX, FLT_MAX));
    
    static ImPlotAxisFlags flags = ImPlotAxisFlags_LockMin;
    
    
    static float t = 0;
    //t += ImGui::GetIO().DeltaTime;
    t = plot_update_interval;
    
    sensor_plots.resize(sensor_count);
    for(int i = 0; i < sensor_count; i++)
    {
        auto& s = sensor[i];
        sensor_plots[i].AddPoint(t, s.current_value * 0.005f);
    }
    
    ImGui::Begin("##Sensor_Plot", NULL); // ImGuiWindowFlags_NoCollapse
    
    if(ImPlot::BeginPlot("##Scrolling", ImVec2(-1,-1), ImPlotFlags_NoLegend))
    {
        ImPlot::SetupAxes("Time", "Values", flags, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - 30.0f, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,0,10);
        for(int i = 0; i < sensor_count; i++)
        {
            auto& s = sensor[i];
            ImPlot::SetNextLineStyle(sensor_graph_color[i],0.5f);
            
            ImGui::PushID(i);
            if(sensor_graph_enabled[i])
                ImPlot::PlotLine(s.name.c_str(), &sensor_plots[i].Data[0].x, &sensor_plots[i].Data[0].y, sensor_plots[i].Data.size(), 0, sensor_plots[i].Offset, 2*sizeof(float));
            ImGui::PopID();
        }
        ImPlot::EndPlot();
    }
    ImGui::End();
}

void RenderUI()
{
    // MARK: Main Menu Bar
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("Psensor"))
        {
            if(ImGui::MenuItem("Preferences"))
            {
                preferences = true;
            }
            
            if(ImGui::MenuItem("Fan Controller"))
            {
                if(Utils::is_root())
                {
                    // idk why but this feels like it can fail
                    EnableFanPwmOnce();
                    if(!PCFans || PCFans->fanInputCount <= 0)
                    {
                        log_error("No fans detected.");
                        no_fc_available_mod = true;
                    }
                    else
                        fan_controller_open = true; 
                }
                else
                {
                    log_error("Not running as root");
                    no_root_open = true;
                }
            }
            if(ImGui::MenuItem("Sensor Settings"))
            {
                sensor_settings = true;
            }
            ImGui::Separator();
            if(ImGui::MenuItem("Exit"))
            {
                loop_exit = true;
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Help"))
        {
            if(ImGui::MenuItem("About"))
            {
                about_window = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
    // MARK: Open Modals
    
    if(no_root_open)
        ImGui::OpenPopup("Fan Controller Error");
    
    
    if(no_fc_available_mod)
        ImGui::OpenPopup("Fan Controller Unavailable");
    
    
    //MARK: Modals
    
    if(ImGui::BeginPopupModal("Fan Controller Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Fan controller only works as root.\nPlease run as root.");
    
        ImGui::SetCursorPos(ImVec2(80, 100));
        if(ImGui::Button("Ok", ImVec2(120, 0)))
        {
            no_root_open = false;
            ImGui::CloseCurrentPopup();
        }
    
        ImGui::EndPopup();
    }
    
    if(ImGui::BeginPopupModal("Fan Controller Unavailable", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("No fans could be detected.\nPlease make sure to load the following kernel modules:\n");
        ImGui::BulletText("nct6775    (General Super I/O Chips)");
        ImGui::BulletText("it87              (ITE Chips)");
        ImGui::BulletText("coretemp (Intel)");
        ImGui::BulletText("k10temp   (AMD)");
    
        ImGui::SetCursorPos(ImVec2(160, 235));
        if(ImGui::Button("Ok", ImVec2(120, 0)))
        {
            no_fc_available_mod = false;
            ImGui::CloseCurrentPopup();
        }
    
        ImGui::EndPopup();
    }

    
    // MARK: UI Body
    if(fan_controller_open)
        RenderFanControllerWindow();
    
    if(about_window)
        RenderAboutWindow();
    
    if(sensor_settings)
        RenderSensorSettings();
    
    if(preferences)
        RenderPreferences();
    
    RenderSensorList();
    RenderSensorPlot();
}