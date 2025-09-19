// C++ don't mangle my C functions !!! >:(
extern "C"
{
    #include <psensor/psensor.h>
    #include <psensor/fan_control.h>
}

#include <vector>
#include <sstream>
#include <mutex>
#include <map>
#include <future>
#include <chrono>


#include <log_c/log.h>

#include <imgui/imgui.h>
#include <imgui/imgui_styles.h>


#include "ui.hpp"
#include "settings_utils.hpp"
#include "sensor_list.hpp"

#include "../utils.hpp"
#include "../ui_entry.hpp"










psensor_fan *PCFans = {NULL};
std::map<int, std::future<void>> fanTestFutures;
std::once_flag pwm_flag;
std::once_flag fc_close_flag;
inline std::vector<bool> is_fan_test_running;
inline std::vector<int> fan_pwm;
inline std::vector<int> prev_fan_pwm;
inline std::vector<bool> pwm_set_exe;





// MARK: Internal funcs

ImVec4 ImVec4_RGBtoFloat(ImVec4 c)
{
    return ImVec4(
        c.x / 255.0f,
        c.y / 255.0f,
        c.z / 255.0f,
        c.w / 255.0f 
    );
}

RGBA_int FloatRGB2Int(ImVec4 c)
{
    return
    {
        (int)(c.x * 255.0f + 0.5f),
        (int)(c.y * 255.0f + 0.5f),
        (int)(c.z * 255.0f + 0.5f),
        (int)(c.w * 255.0f + 0.5f)
    };
}

// Stolen from NVi-PFA :madman:
std::string FilenameOnly(const std::string& path) 
{
    size_t slash = path.find_last_of("/\\");
    return (slash == std::string::npos) ? path : path.substr(slash + 1);
}



void RefreshSensorList()
{
    // Stop the sensor updater loop
    keep_sensor_update.store(false);

    // Wait till the sensor updater finishes
    if(sensor_update_thr.joinable())
    {
        sensor_update_thr.join();
    }

    // Free psensor list
    {
        std::lock_guard<std::mutex> lock_sensors(sensors_mutex);
        if(sensors)
        {
            psensor_list_free(sensors);
            sensors = nullptr;
            sensor_count = 0;
        }
    }

    // Clear plots and previous sensor data
    {
        std::lock_guard<std::mutex> lock_list(sensor_list_mutex);
        for(auto &p : sensor_plots)
        {
            p.Erase();
        }
        sensor_plots.clear();
        sensor.clear();
    }
    
    // Reset plot update interval to 0
    plot_update_interval = 0.0f;

    // Resync atomic bools
    sensor_list_lmsensors = liveSettings.provider_lmsensors;
    sensor_list_atasmart  = liveSettings.provider_atasmart;
    sensor_list_udisks2   = liveSettings.provider_udisks2;
    sensor_list_gtop      = liveSettings.provider_gtop;
    sensor_list_amd       = liveSettings.provider_amd;
    sensor_list_nvidia    = liveSettings.provider_nvidia;
    sensor_list_hddtemp   = liveSettings.provider_hddtemp;
    sensor_list_up_interv = liveSettings.update_interval;

    // Create the new sensor list (which can differ depending on the enabled providers)
    {
        std::lock_guard<std::mutex> lock_sensors(sensors_mutex);
        create_sensor_list();
    }
    
    // Start the sensor updater thread
    keep_sensor_update.store(true);
    sensor_update_thr = std::thread(update_sensor_list, std::ref(sensor));
}

void EnableFanPwmOnce()
{
    std::call_once(pwm_flag, []()
    {
        PCFans = psensor_detectFans();
        
        psensor_fan_open(PCFans);
        
        if(!PCFans || PCFans->fanInputCount <= 0)
        {
            log_error("Failed to enable fan PWM");
            return;
        }
        for(int i = 0; i < PCFans->fanInputCount; i++)
        {
            psensor_enable_fan_pwm(PCFans, i, 1);
        }
        
        fan_pwm.resize(PCFans->fanInputCount, 0);
        prev_fan_pwm.resize(PCFans->fanInputCount, 0);
        pwm_set_exe.resize(PCFans->fanInputCount, false);
        is_fan_test_running.resize(PCFans->fanInputCount, false);
    });
}

void CloseFanControllerOnce()
{
    std::call_once(fc_close_flag, []()
    {
        if(PCFans != NULL)
            psensor_fan_close(PCFans);
    });
}

void SetFanPwmOnce(int index)
{
    // Ewww That's ugly
    // I hate to say but it f works ;(
    if(!pwm_set_exe[index])
    {
        psensor_fan_set_pwm(PCFans, index, prev_fan_pwm[index]);
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
    
    
    if(ImGui::BeginTable("##FanCtrlTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
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
            
            fan_pwm[row] = psensor_get_last_pwm(PCFans, row);
            
            if(ImGui::SliderInt("##SpeedCtrl", &fan_pwm[row], 0, 255))
            {
                psensor_fan_set_pwm(PCFans, row, fan_pwm[row]);
            }
            
            ImGui::TableSetColumnIndex(2);
            if(ImGui::Button("Test"))
            {
                prev_fan_pwm[row] = fan_pwm[row];
                fanTestFutures[row] = std::async(std::launch::async, [row]()
                {
                    psensor_test_fan(PCFans, row);
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

void AddQuestionMarkTooltip(std::string tooltip_text)
{
    int text_id = 0;
    ImGui::PushID(text_id++);
    ImGui::TextDisabled("(?)");
    ImGui::PopID();
    
    ImGui::PushID(text_id++);
    if(ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::Text("%s", tooltip_text.c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
    ImGui::PopID();
}

void RenderPreferences()
{
    std::ostringstream save_settings_btn_text;
    
    if(autosave_settings)
        save_settings_btn_text << "Autosave";
    else
        save_settings_btn_text << "Save Settings";
    
    ImGui::SetNextWindowSizeConstraints(ImVec2(705, 513), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin("Preferences", &preferences);
    
    ImGui::Text("Settings marked with * apply after restarting the application");
    ImGui::Text("Settings marked with # are not implemented");
    
    ImGui::Separator();
    
    ImGui::BeginDisabled(autosave_settings);
    if(ImGui::Button(save_settings_btn_text.str().c_str()))
        writeConfig(liveSettings);
    
    ImGui::EndDisabled();
    if(ImGui::BeginTabBar("##Settings", 0))
    {
        if(ImGui::BeginTabItem("General"))
        {
            ImGui::Text("* Graphics Platform: ");
            ImGui::SameLine();
            if(ImGui::BeginCombo("##gplat", graphics_plaforms[selected_graphics_platform], ImGuiComboFlags_WidthFitPreview))
            {
                for(int n = 0; n < IM_ARRAYSIZE(graphics_plaforms); n++)
                {
                    const bool is_selected = (selected_graphics_platform == n);
                    if(ImGui::Selectable(graphics_plaforms[n], is_selected))
                    {
                        selected_graphics_platform = n;
                        liveSettings.graphics_platform = selected_graphics_platform;
                        SaveSettings();
                    }
    
                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            AddQuestionMarkTooltip("Set a graphics platform\nAutomatic - tries the available graphics platforms and choses the one that works\nGUI Rendering can be slow when using the software renderer which can be applied if the rest of the graphics platforms are not available");
            if(ImGui::Checkbox("* Save UI layouts", &save_ui_layouts))
            {
                liveSettings.save_ui_layout = save_ui_layouts;
                SaveSettings();
            }
            ImGui::SameLine();
            AddQuestionMarkTooltip("Allow imgui to save UI layouts to ini file");
            
            if(ImGui::Checkbox("Autosave settings", &autosave_settings)) // When disabled it already prevents this setting from saving to json file :madman:
            {
                liveSettings.autosave_settings = autosave_settings;
                SaveSettings();
            }
            
            ImGui::SameLine();
            AddQuestionMarkTooltip("Automatically save the settings on click events");
            
            if(ImGui::Checkbox("Allow Screen Saving", &cb_allow_screen_saver))
            {
                liveSettings.allow_screen_saver = cb_allow_screen_saver;
                SaveSettings();
            }
            
            ImGui::SameLine();
            AddQuestionMarkTooltip("Allows the screen to turn off when idle");
            
            //RGBA_int temp = clear_color;
            ImGui::ColorEdit3("Background Color", (float*)&clear_color);
            liveSettings.bg_color = FloatRGB2Int(clear_color);
            ImGui::SameLine();
            AddQuestionMarkTooltip("Changes the background color of the main window");
            
            if(ImGui::BeginCombo("# UI Theme", FilenameOnly(ui_themes[selected_ui_theme]).c_str(), ImGuiComboFlags_WidthFitPreview))
            {
                for(int n = 0; n < ui_themes_count; n++)
                {
                    const bool is_selected = (selected_ui_theme == n);
                    if(ImGui::Selectable(FilenameOnly(ui_themes[n]).c_str(), is_selected))
                    {
                        selected_ui_theme = n;
                        liveSettings.color_theme_index = selected_ui_theme;
                        
                        if(selected_ui_theme == 0)
                            SetDefaultTheme();
                        else
                            ImGui::LoadStyleFrom(ui_themes[selected_ui_theme]);
                        
                        SaveSettings();
                    }
    
                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Providers"))
        {
            if(ImGui::Button("Refresh Sensor List"))
            {
                RefreshSensorList();
            }
            
            if(ImGui::Checkbox("lm_sensors", &cb_provider_lmsensors))
            {
                liveSettings.provider_lmsensors = cb_provider_lmsensors;
                SaveSettings();
            }
            ImGui::SameLine();
            AddQuestionMarkTooltip("Provides temperature and fan speed information");
            
            if(ImGui::Checkbox("udisks2", &cb_provider_udisks2))
            {
                liveSettings.provider_udisks2 = cb_provider_udisks2;
                SaveSettings();
            }
            ImGui::SameLine();
            AddQuestionMarkTooltip("Provides disk information");
            
            if(ImGui::Checkbox("hddtemp", &cb_provider_hddtemp))
            {
                liveSettings.provider_hddtemp = cb_provider_hddtemp;
                SaveSettings();
            }
            ImGui::SameLine();
            AddQuestionMarkTooltip("Provides disk / SSD temperature");
            
            if(ImGui::Checkbox("atasmart", &cb_provider_atasmart))
            {
                liveSettings.provider_atasmart = cb_provider_atasmart;
                SaveSettings();
            }
            ImGui::SameLine();
            AddQuestionMarkTooltip("Provides S.M.A.R.T. details for storage devices");
            
            if(ImGui::Checkbox("gtop", &cb_provider_gtop))
            {
                liveSettings.provider_gtop = cb_provider_gtop;
                SaveSettings();
            }
            ImGui::SameLine();
            AddQuestionMarkTooltip("Provides system information resources (CPU usage, Memory etc)");
            
            if(ImGui::Checkbox("amd", &cb_provider_amd))
            {
                liveSettings.provider_amd = cb_provider_amd;
                SaveSettings();
            }
            ImGui::SameLine();
            AddQuestionMarkTooltip("Provides AMD GPU information (GPU usage)");
            
            if(ImGui::Checkbox("nvidia", &cb_provider_nvidia))
            {
                liveSettings.provider_nvidia = cb_provider_nvidia;
                SaveSettings();
            }
            ImGui::SameLine();
            AddQuestionMarkTooltip("Provides NVIDIA GPU information");
            
            if(ImGui::Checkbox("bcm2835", &cb_provider_bcm2835))
            {
                liveSettings.provider_bcm2835 = cb_provider_bcm2835;
                SaveSettings();
            }
            ImGui::SameLine();
            AddQuestionMarkTooltip("Provides thermal zone temperature information (Raspberry pi 3)");
            
            ImGui::BeginDisabled();
            ImGui::Checkbox("# IPMI", &cb_provider_ipmi);
            ImGui::EndDisabled();
            
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Sensor List"))
        {
            if(ImGui::Checkbox("Use Celsius Temperature Unit", &cb_use_celsiustemp))
            {
                liveSettings.use_celsius_temp_unit = cb_use_celsiustemp;
                SaveSettings();
            }
            ImGui::SameLine();
            AddQuestionMarkTooltip("When disabled the temperature unit is transformed to Fahrenheit");
            
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Plot settings"))
        {
            ImGui::PushID("##Refresh Sensor List");
            if(ImGui::Button("Refresh Sensor List"))
            {
                RefreshSensorList();
            }
            ImGui::PopID();
            if(ImGui::SliderInt("* Plot Buffer Size", &sl_plot_buf_size, 1000, 50000))
            {
                liveSettings.scroll_buffer_size = sl_plot_buf_size;
            }
            ImGui::SameLine();
            AddQuestionMarkTooltip("Set how many plot points to keep in memory");
            
            if(ImGui::SliderFloat("Plot History", &sl_plot_buf_history, 10.0f, 60.0f))
            {
                liveSettings.scroll_buffer_history = sl_plot_buf_history;
            }
            ImGui::SameLine();
            AddQuestionMarkTooltip("Set how much plot history to view");
            
            if(ImGui::SliderInt("Update Interval", &sl_update_interval, 500, 5000))
            {
                liveSettings.update_interval = sl_update_interval;
            }
            ImGui::SameLine();
            AddQuestionMarkTooltip("Set the time interval for updating sensor list / plots");
            
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Fan Controller"))
        {
            if(ImGui::Checkbox("# Skip kernel module loading", &cb_skip_mod_load))
            {
                liveSettings.skip_module_loading = cb_skip_mod_load;
                SaveSettings();
            }
            ImGui::SameLine();
            AddQuestionMarkTooltip("Kernel module loading is generally required in order to detect fans in sysfs\nKeep it enabled if no fans could be detected");
            
            if(ImGui::Checkbox("# Emergency Cooling", &cb_emergency_cooling))
            {
                liveSettings.emergency_cooling = cb_emergency_cooling;
                SaveSettings();
            }
            ImGui::SameLine();
            AddQuestionMarkTooltip("Enable the builtin emergency cooling.\nIt works by monitoring CPU and GPU temperatures and sets high PWM values to prevent overheating");
            
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
    
    if(ImGui::BeginTable("sensor_table", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
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
            ImGui::Text("%s", psensor_value_to_str(s.sensor_type, s.current_value, liveSettings.use_celsius_temp_unit));
            
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%s", psensor_value_to_str(s.sensor_type, s.min, liveSettings.use_celsius_temp_unit));
            
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%s", psensor_value_to_str(s.sensor_type, s.max, liveSettings.use_celsius_temp_unit));
            
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
        sensor_plots[i].AddPoint(t, s.current_value * 0.180f);
    }
    
    ImGui::Begin("##Sensor_Plot", NULL); // ImGuiWindowFlags_NoCollapse
    
    if(ImPlot::BeginPlot("##Scrolling", ImVec2(-1,-1), ImPlotFlags_NoLegend))
    {
        ImPlot::SetupAxes("Time", "Values", flags, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,t - sl_plot_buf_history, t, ImGuiCond_Always);
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
            if(ImGui::MenuItem("Refresh Sensor List"))
            {
                RefreshSensorList();
            }
            
            if(ImGui::MenuItem("Fan Controller"))
            {
                if(Utils::is_root())
                {
                    EnableFanPwmOnce();
                    // idk why but this feels like it can fail
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
            
            ImGui::Separator();
            if(ImGui::MenuItem("Exit"))
            {
                loop_exit = true;
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Edit"))
        {
            if(ImGui::MenuItem("Preferences"))
            {
                preferences = true;
            }
            
            if(ImGui::MenuItem("Sensor Settings"))
            {
                sensor_settings = true;
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
    else
        CloseFanControllerOnce();
    
    if(about_window)
        RenderAboutWindow();
    
    if(sensor_settings)
        RenderSensorSettings();
    
    if(preferences)
        RenderPreferences();
    
    RenderSensorList();
    RenderSensorPlot();
}