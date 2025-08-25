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

#include "imgui/imgui.h"


#include "ui.hpp"

#include "../utils.hpp"










psensor_fan *PCFans;
std::map<int, std::future<void>> fanTestFutures;
std::once_flag pwm_flag;
inline std::vector<bool> is_fan_test_running;
inline std::vector<int> fan_pwm;
inline std::vector<int> prev_fan_pwm;
inline std::vector<bool> pwm_set_exe;


// MARK: Fan Controller

void EnableFanPwmOnce()
{
    std::call_once(pwm_flag, []()
    {
        PCFans = psensor_detectFans();
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
        log_trace("Fan PWM %d", prev_fan_pwm[index]);
        psensor_fan_set_pwm(PCFans->pwmFiles[index], prev_fan_pwm[index]);
        fan_pwm[index] = prev_fan_pwm[index]; // sync UI with restored value
        pwm_set_exe[index] = true;
    }
}


void RenderFanControllerWindow()
{
    EnableFanPwmOnce();
    
    ImGuiIO& io = ImGui::GetIO();
    
    ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
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
            
            fan_pwm[row] = psensor_get_last_pwm(PCFans->pwmFiles[row]);
            
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Fan %d", row);
            ImGui::TableSetColumnIndex(1);
            ImGui::BeginDisabled(is_fan_test_running[row]);
            
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
                    log_info("Fan %d test done!", row);
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

void RenderUI()
{
    // MARK: Main Menu Bar
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("Psensor"))
        {
            if(ImGui::MenuItem("Preferences"))
            {
                log_info("Preferences");
            }
            
            if(ImGui::MenuItem("Fan Controller"))
            {
                if(Utils::is_root())
                    fan_controller_open = true;
                else
                {
                    log_error("Not running as root");
                    no_root_open = true;
                }
            }
            if(ImGui::MenuItem("Sensor Settings"))
            {
                log_info("Sensor Settings");
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
                log_info("About");
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
    // MARK: Open Modals
    
    if(no_root_open)
    {
        ImGui::OpenPopup("Fan Controller Error");
    }
    
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

    
    // MARK: UI Body
    if(fan_controller_open)
    {
        RenderFanControllerWindow();
    }
}