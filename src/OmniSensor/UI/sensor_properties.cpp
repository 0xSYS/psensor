#include <fstream>
#include <sys/stat.h>
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

    int fd = open(out_path.str().c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    
    if(fd == -1)
    {
        log_error("open() failed: %s", strerror(errno));
        return;
    }
    
    fchmod(fd, 0666);
    
    FILE* f = fdopen(fd, "w");
    if(!f)
    {
        log_error("fdopen() failed: %s", strerror(errno));
        close(fd);
        return;
    }
        
    std::ofstream out_file(out_path.str());
    
    if(!out_file)
        log_error("Output file stream failed: %s", strerror(errno));
    
    out_file << json_out.dump(4);
    out_file.close();
}

std::vector<ui_sensor> loadSensorProperties()
{
    std::vector<ui_sensor> sensor_props_out;
    std::ostringstream out_path;
    out_path << Utils::get_home_dir() << SENSOR_PROPERTIES_PATH;
    std::ifstream in_conf;
    
   
    in_conf = std::ifstream(out_path.str());
    
    if(!in_conf.is_open())
    {
        log_warn("Sensor properties file not found, returning empty list");
        return sensor_props_out;
    }
    
    nlohmann::json json_in = nlohmann::json::parse(in_conf);
    
    
    for(auto& js : json_in)
    {
        ui_sensor sensor;
        sensor.sensor_id     = js.value("id", "");
        sensor.name          = js.value("name", "");
        sensor.graph_visible = js.value("showPlot", true);
    
        auto col = js.value("plotColor", std::vector<int>{0,0,0,255});
        if(col.size() < 4)
            col.resize(4, 255);
        
        ImVec4 c_int(
            static_cast<float>(col[0]),
            static_cast<float>(col[1]),
            static_cast<float>(col[2]),
            static_cast<float>(col[3])
        );
        
        sensor.graph_color = c_int;
    
        sensor_props_out.push_back(sensor);
    }
    
    
    return sensor_props_out;
}