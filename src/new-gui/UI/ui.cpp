// C++ don't mangle my C functions !!! >:(
extern "C"
{
    #include <psensor/psensor.h>
}

#include <mutex>


#include <log_c/log.h>

#include "imgui/imgui.h"


#include "ui.hpp"

#include "../utils.hpp"










psensor_fan *PCFans;
std::once_flag pwm_flag;


void EnableFanPwmOnce()
{
    std::call_once(pwm_flag, []()
    {
        log_trace("Once only");
        PCFans = psensor_detectFans();
        for(int i = 0; i < PCFans->fanInputCount; i++)
        {
            psensor_enable_fan_pwm(PCFans->pwmEnableFiles[i], 1);
        }
    });
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
    
    
    if (ImGui::BeginTable("table_item_width", 3, ImGuiTableFlags_Borders))
    {
        ImGui::TableSetupColumn("Fan Index");
        ImGui::TableSetupColumn("Speed (PWM)");
        ImGui::TableSetupColumn("Speed Test");
        ImGui::TableHeadersRow();
        
        for (int row = 0; row < PCFans->fanInputCount; row++)
        {
            ImGui::TableNextRow();
            if (row == 0)
            {
                ImGui::TableSetColumnIndex(0);
                ImGui::PushItemWidth(TEXT_BASE_WIDTH * 3.0f);
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::TableSetColumnIndex(2);
                ImGui::PushItemWidth(-FLT_MIN);
            }

            // Draw our contents
            static int dummy_f = 0;
            ImGui::PushID(row);
            
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Fan %d", row);
            ImGui::TableSetColumnIndex(1);
            if(ImGui::SliderInt("##SpeedCtrl", &dummy_f, 0, 255))
            {
                psensor_fan_set_pwm(PCFans->pwmFiles[row], dummy_f);
            }
            
            ImGui::TableSetColumnIndex(2);
            if(ImGui::Button("Test"))
            {
                psensor_test_fan(PCFans->pwmFiles[row]);
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
    
    if(no_root_open)
    {
        ImGui::OpenPopup("Fan Controller Error");
    }
    
    //MARK: Modals
    // Call every frame
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