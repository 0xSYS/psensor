#include <nlohmann/json.hpp>
#include <fstream>

#include <log_c/log.h>



#include "config_utils.hpp"
#include "utils.hpp"






pconfig readConfig()
{
    pconfig out_conf;
    std::ostringstream out_path;
    out_path << Utils::get_home_dir() << PSENSOR_SETTINGS_PATH;
    
    std::ifstream in_conf(out_path.str());
    in_conf.close();
    
    nlohmann::json json_in = nlohmann::json::parse(in_conf);
    
    
    nlohmann::json general_obj = json_in["general"];
    
    out_conf.graphics_platform = general_obj["graphicsPlatform"].get<std::string>();
    
    
    nlohmann::json providers_obj = json_in["providers"];
    
    out_conf.provider_gtop      = providers_obj["gtop"].get<bool>();
    out_conf.provider_lmsensors = providers_obj["lmsensors"].get<bool>();
    out_conf.provider_udisks2   = providers_obj["udisks2"].get<bool>();
    out_conf.provider_atasmart  = providers_obj["atasmart"].get<bool>();
    out_conf.provider_hddtemp   = providers_obj["hddtemp"].get<bool>();
    out_conf.provider_amd       = providers_obj["amd"].get<bool>();
    out_conf.provider_nvidia    = providers_obj["nvidia"].get<bool>();
    
    
    nlohmann::json ui_settings_obj = json_in["uiSettings"];
    
    out_conf.save_ui_layout = ui_settings_obj["saveUIlayouts"].get<bool>();
    
    
    nlohmann::json main_window_obj = ui_settings_obj["mainWindow"];
    
    out_conf.window_w = main_window_obj["width"].get<int>();
    out_conf.window_h = main_window_obj["height"].get<int>();
    
    nlohmann::json plot_settings_obj = json_in["plotSettings"];
    
    out_conf.scroll_buffer_size    = plot_settings_obj["scrollBuffer"].get<int>();
    out_conf.scroll_buffer_history = plot_settings_obj["scrollBufferHistory"].get<float>();
    out_conf.update_interval       = plot_settings_obj["updateInterval"].get<float>();
    
    
    nlohmann::json fan_controller_obj = json_in["fanController"];
    out_conf.skip_module_loading      = fan_controller_obj["skipModuleLoading"].get<bool>();
    out_conf.emergency_cooling        = fan_controller_obj["emergencyCooling"].get<bool>();
    out_conf.cooling_preset           = fan_controller_obj["coolingPreset"].get<std::string>();
    
    return out_conf;
}


void writeConfig(const pconfig config)
{
    nlohmann::json json_out;
    std::ostringstream out_path;
    
    json_out =
    {
        { 
            "general",
            {
                { "graphicsPlatform", config.graphics_platform }
            }
        },
        {
            "providers",
            {
                { "lmsensors", config.provider_lmsensors },
                { "udisks2",   config.provider_udisks2   },
                { "gtop",      config.provider_gtop      },
                { "atasmart",  config.provider_atasmart  },
                { "hddtemp",   config.provider_hddtemp   },
                { "amd",       config.provider_amd       },
                { "nvidia",    config.provider_nvidia    }
            }
        },
        {
            "uiSettings",
            {
                { "saveUIlayouts", config.save_ui_layout },
                {
                    "mainWindow",
                    {
                        { "width",  config.window_w },
                        { "height", config.window_h }
                    }
                }
            }
        },
        {
            "plotSettings",
            {
                { "scrollBuffer",        config.scroll_buffer_size    },
                { "scrollBufferHistory", config.scroll_buffer_history },
                { "updateInterval",      config.update_interval       }
            }
        },
        {
            "fanController",
            {
                { "skipModuleLoading", config.skip_module_loading },
                { "emergencyCooling",  config.emergency_cooling   },
                { "coolingPresets",    config.cooling_preset      }
            }
        }
    };
    
    out_path << Utils::get_home_dir() << PSENSOR_SETTINGS_PATH;
    std::ofstream out_file(out_path.str());
    out_file << json_out.dump(4);
    out_file.close();
    log_info("Configuration saved");
}

void printConfig(const pconfig config)
{
    // 
}