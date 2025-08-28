#include <vector>
#include <thread>
#include <mutex>
#include <unistd.h>



#include "imgui/imgui.h"


#include "sensor_list.hpp"

extern "C"
{
    #include <psensor/psensor.h>
    #include <psensor/lmsensor.h>
    #include <psensor/hdd.h>
    #include <psensor/nvidia.h>
    #include <psensor/amd.h>
    #include <psensor/pgtop2.h>
    #include <psensor/pudisks2.h>
    #include <psensor/psensor_json.h>
    #include <log_c/log.h>
}




struct psensor **sensors = nullptr;



ImVec4 ImVec4_RGBtoFloat(ImVec4 c)
{
    return ImVec4(c.x / 255.0f, c.y / 255.0f, c.z / 255.0f, c.w / 255.0f);
}


void create_sensor_list()
{
    lmsensor_psensor_list_append(&sensors, 600);
    hddtemp_psensor_list_append(&sensors, 600);
    atasmart_psensor_list_append(&sensors, 600);
    nvidia_psensor_list_append(&sensors, 600);
    amd_psensor_list_append(&sensors, 600);
    gtop2_psensor_list_append(&sensors, 600);
    udisks2_psensor_list_append(&sensors, 600);
    sensor_list_created = true;
}

void update_sensor_list(std::vector<ui_sensor>& sl)
{
    create_sensor_list(); // Temporary
    if(!sensor_list_created)
    {
        log_error("Sensor list not created");
        return;
    }

    while(1)
    {
        lmsensor_psensor_list_update(sensors);
        nvidia_psensor_list_update(sensors);
        amd_psensor_list_update(sensors);
        udisks2_psensor_list_update(sensors);
        gtop2_psensor_list_update(sensors);
        atasmart_psensor_list_update(sensors);
        hddtemp_psensor_list_update(sensors);

        std::vector<ui_sensor> temp_list;
        for(int i = 0; sensors[i] != nullptr; i++)
        {
            struct psensor* s = sensors[i];
            temp_list.emplace_back(
                s->name,
                psensor_get_current_value(s),
                s->sess_lowest,
                s->sess_highest,
                graph_colors[i],
                false
            );
        }

        {
            std::lock_guard<std::mutex> lock(sensor_list_mutex);
            sl = std::move(temp_list);
            sensor_count = sl.size();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
