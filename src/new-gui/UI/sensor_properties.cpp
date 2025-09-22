#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <nlohmann/json.hpp>
#include <log_c/log.h>


#include "sensor_properties.hpp"
#include "../utils.hpp"
#include "ui.hpp"




void saveSensorProperties(std::vector<ui_sensor>& sens_props)
{
    nlohmann::ordered_json json_out = nlohmann::json::array();
    std::ostringstream out_path;
    
    for(auto& sensor : sens_props)
    {
        nlohmann::ordered_json json_sensor = nlohmann::json::object();
        json_sensor["id"] = sensor.sensor_id;
        json_sensor["name"] = sensor.name;
        json_sensor["showPlot"] = sensor.graph_visible;
        RGBA_int rgba_col = FloatRGB2Int(sensor.graph_color);
        json_sensor["plotColor"] = {rgba_col.r, rgba_col.g, rgba_col.b, rgba_col.a};
        json_out.push_back(json_sensor);
    }
    
    out_path << Utils::get_home_dir() << SENSOR_PROPERTIES_PATH;
    
    // Some attempt to make the configuration file to be writable by the user regardless of being root or not
    int fd = open(out_path.str().c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    
    if(fd == -1)
    {
        log_error("open() failed: %s", strerror(errno));
        return;
    }
    
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
}

std::vector<ui_sensor> loadSensorProperties()
{
    std::vector<ui_sensor> sensor_props_out;
    std::ostringstream out_path;
    out_path << Utils::get_home_dir() << SENSOR_PROPERTIES_PATH;
    
    
    std::ifstream in_conf(out_path.str());
    
    nlohmann::json json_in = nlohmann::json::parse(in_conf);
    
    
    for(auto& js : json_in)
    {
        ui_sensor sensor;
        sensor.sensor_id     = js.at("id").get<int>();
        sensor.name          = js.at("name").get<std::string>();
        sensor.graph_visible = js.at("showPlot").get<bool>();
    
        // plotColor is an array -> index directly
        auto col = js.value("plotColor", std::vector<int>{0,0,0,255});
        if (col.size() < 4) col.resize(4, 255); // ensure alpha present
        
        // construct ImVec4 with integer components
        ImVec4 c_int(
            static_cast<float>(col[0]),
            static_cast<float>(col[1]),
            static_cast<float>(col[2]),
            static_cast<float>(col[3])
        );
        
        // now convert to normalized float ImVec4
        sensor.graph_color = ImVec4_RGBtoFloat(c_int);
    
        sensor_props_out.push_back(sensor);
    }
    
    
    return sensor_props_out;
}