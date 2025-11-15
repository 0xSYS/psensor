/*
 * Copyright (C) 2025 kiptunor
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */








#include <iostream>



#include <nlohmann/json.hpp>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <log_c/log.h>



#include "config_utils.hpp"
#include "utils.hpp"
#include "UI/settings_utils.hpp"






pconfig readConfig()
{
    pconfig out_conf;
    std::ostringstream out_path;
    out_path << Utils::get_home_dir() << PSENSOR_SETTINGS_PATH;
    
    log_info("Reading config...");
    
    std::ifstream in_conf(out_path.str());
    
    nlohmann::json json_in = nlohmann::json::parse(in_conf);
    
    
    nlohmann::json general_obj = json_in.value("general", nlohmann::json::object());
    
    out_conf.graphics_platform  = general_obj.value("graphicsPlatform", defaultSettings.graphics_platform);
    out_conf.autosave_settings  = general_obj.value("autosaveSettings", defaultSettings.autosave_settings);
    out_conf.allow_screen_saver = general_obj.value("allowScreenSaver", defaultSettings.allow_screen_saver);
    
    nlohmann::json sensor_list_obj = json_in.value("sensorList", nlohmann::json::array());
    
    out_conf.use_celsius_temp_unit = sensor_list_obj["useCelsiusTempUnit"].get<bool>();
    
    
    nlohmann::json providers_obj = json_in.value("providers", nlohmann::json::object());
    
    out_conf.provider_gtop      = providers_obj.value("gtop",      defaultSettings.provider_gtop     );
    out_conf.provider_lmsensors = providers_obj.value("lmsensors", defaultSettings.provider_lmsensors);
    out_conf.provider_udisks2   = providers_obj.value("udisks2",   defaultSettings.provider_udisks2  );
    out_conf.provider_atasmart  = providers_obj.value("atasmart",  defaultSettings.provider_atasmart );
    out_conf.provider_hddtemp   = providers_obj.value("hddtemp",   defaultSettings.provider_hddtemp  );
    //out_conf.provider_amd       = providers_obj.value("amd",       defaultSettings.provider_amd      );
    out_conf.provider_roc_smi   = providers_obj.value("roc-smi",   defaultSettings.provider_roc_smi  );
    out_conf.provider_nvidia    = providers_obj.value("nvidia",    defaultSettings.provider_nvidia   );
    out_conf.provider_bcm2835   = providers_obj.value("bcm2835",   defaultSettings.provider_bcm2835  );
    out_conf.provider_ipmi      = providers_obj.value("ipmi",      defaultSettings.provider_ipmi     );
    
    
    nlohmann::json ui_settings_obj = json_in.value("uiSettings", nlohmann::json::object());
    
    out_conf.save_ui_layout    = ui_settings_obj.value("saveUIlayouts", defaultSettings.save_ui_layout);
    out_conf.ui_font_size      = ui_settings_obj.value("uiFontSize", defaultSettings.ui_font_size);
    out_conf.ui_theme_path     = ui_settings_obj.value("uiTheme", defaultSettings.ui_theme_path);
    
    
    nlohmann::json main_window_obj = ui_settings_obj.value("mainWindow", nlohmann::json::object());
    
    out_conf.window_w = main_window_obj.value("width", defaultSettings.window_w);
    out_conf.window_h = main_window_obj.value("height", defaultSettings.window_h);
    
    nlohmann::json bg_color_obj = main_window_obj.value("backgroundColor", nlohmann::json::object());
    
    out_conf.bg_color.r = bg_color_obj.value("r", defaultSettings.bg_color.r);
    out_conf.bg_color.g = bg_color_obj.value("g", defaultSettings.bg_color.g);
    out_conf.bg_color.b = bg_color_obj.value("b", defaultSettings.bg_color.b);
    out_conf.bg_color.a = bg_color_obj.value("a", defaultSettings.bg_color.a);
    
    
    nlohmann::json plot_settings_obj = json_in.value("plotSettings", nlohmann::json::object());
    
    out_conf.scroll_buffer_size    = plot_settings_obj.value("scrollBuffer", defaultSettings.scroll_buffer_size);
    out_conf.scroll_buffer_history = plot_settings_obj.value("scrollBufferHistory", defaultSettings.scroll_buffer_history);
    out_conf.update_interval       = plot_settings_obj.value("updateInterval", defaultSettings.update_interval);
    

    nlohmann::json fan_controller_obj = json_in.value("fanController", nlohmann::json::object());
    
    out_conf.skip_module_loading      = fan_controller_obj.value("skipModuleLoading", defaultSettings.skip_module_loading);
    out_conf.emergency_cooling        = fan_controller_obj.value("emergencyCooling", defaultSettings.emergency_cooling);
    out_conf.cooling_preset           = fan_controller_obj.value("coolingPreset", defaultSettings.cooling_preset);
    
    in_conf.close();
    
    return out_conf;
}


void writeConfig(const pconfig config)
{
    /*
    Todo: Fix file permissions
    */
    nlohmann::ordered_json json_out;
    std::ostringstream out_path;
    
    json_out =
    {
        { 
            "general",
            {
                { "graphicsPlatform", config.graphics_platform },
                { "autosaveSettings", config.autosave_settings },
                { "allowScreenSaver", config.allow_screen_saver }
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
                //{ "amd",       config.provider_amd       },
                { "roc-smi",   config.provider_roc_smi   },
                { "nvidia",    config.provider_nvidia    },
                { "bcm2835",   config.provider_bcm2835   },
                { "ipmi",      config.provider_ipmi      }
            }
        },
        {
            "sensorList",
            {
                { "useCelsiusTempUnit", config.use_celsius_temp_unit }
            }
        },
        {
            "uiSettings",
            {
                { "saveUIlayouts", config.save_ui_layout },
                { "uiFontSize",    config.ui_font_size },
                { "uiTheme", config.ui_theme_path },
                { "mainWindow",
                    {
                        { "width", config.window_w },
                        { "height", config.window_h },
                        {
                            "backgroundColor",
                            {
                                { "r", config.bg_color.r },
                                { "g", config.bg_color.g },
                                { "b", config.bg_color.b },
                                { "a", config.bg_color.a }
                            }
                        }
                    }
                }
            }
        },
        {
            "plotSettings",
            {
                { "scrollBuffer",        config.scroll_buffer_size    },
                { "scrollBufferHistory", config.scroll_buffer_history },
                { "updateInterval",      config.update_interval       },
            }
        },
        {
            "fanController",
            {
                { "skipModuleLoading", config.skip_module_loading },
                { "emergencyCooling",  config.emergency_cooling   },
                { "coolingPreset",     config.cooling_preset      }
            }
        }
    };
    
    out_path << Utils::get_home_dir() << PSENSOR_SETTINGS_PATH;
    
    // Some attempt to make the configuration file to be writable by the user regardless of being root or not
    int fd = open(out_path.str().c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    
    if(fd == -1)
    {
        log_error("open() failed: %s", strerror(errno));
        return;
    }
    
    fchmod(fd, 0666);
    
    // Convert fd → stream
    FILE* f = fdopen(fd, "w");
    if(!f)
    {
        log_error("fdopen() failed: %s", strerror(errno));
        close(fd);
        return;
    }
        
    std::ofstream out_file(out_path.str());
    out_file << json_out.dump(4);
    out_file.close();
    log_info("Configuration saved");
}

void printConfig(const pconfig config)
{
    std::cout << "settings from struct:\n";
    std::cout << "[INT]  - window_w:           -> " << config.window_w << "\n";
    std::cout << "[INT]  - window_h:           -> " << config.window_h << "\n";
    std::cout << "[BOOL] - autosave_settings:  -> " << config.autosave_settings << "\n";
    std::cout << "[BOOL] - allow_screen_saver: -> " << config.allow_screen_saver << "\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "[STRING] - graphics_platform: "    << config.graphics_platform << "\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "[BOOL] - provider_gtop:      -> " << config.provider_gtop      << "\n";
    std::cout << "[BOOL] - provider_lmsensors: -> " << config.provider_lmsensors << "\n";
    std::cout << "[BOOL] - provider_udisks2:   -> " << config.provider_udisks2   << "\n";
    std::cout << "[BOOL] - provider_atasmart:  -> " << config.provider_atasmart  << "\n";
    std::cout << "[BOOL] - provider_hddtemp:   -> " << config.provider_hddtemp   << "\n";
    std::cout << "[BOOL] - provider_roc_smi:   -> " << config.provider_roc_smi   << "\n";
    std::cout << "[BOOL] - provider_nvidia:    -> " << config.provider_nvidia    << "\n";
    std::cout << "[BOOL] - provider_ipmi:      -> " << config.provider_ipmi      << "\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "[BOOL]   - save_ui_layout:   -> " << config.save_ui_layout << "\n";
    std::cout << "[FLOAT]  - ui_font_size:     -> " << config.ui_font_size   << "\n";
    std::cout << "[STRING] - color_theme_path: -> " << config.ui_theme_path  << "\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "[BOOL] - use_celsius_temp_unit: -> " << config.use_celsius_temp_unit << "\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "[INT]   - scroll_buffer_size:    -> " << config.scroll_buffer_size    << "\n";
    std::cout << "[FLOAT] - scroll_buffer_history: -> " << config.scroll_buffer_history << "\n";
    std::cout << "[INT]   - update_interval:       -> " << config.update_interval       << "\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "[BOOL]   - skip_module_loading: -> "  << config.skip_module_loading << "\n";
    std::cout << "[BOOL]   - emergency_cooling:   -> "  << config.emergency_cooling   << "\n";
    std::cout << "[STRING] - cooling_preset:      -> "  << config.cooling_preset      << "\n";
}