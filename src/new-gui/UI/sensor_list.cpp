#include <vector>
#include <thread>
#include <mutex>
#include <unistd.h>

#include <SDL3/SDL.h>



#include <imgui/imgui.h>


#include "sensor_list.hpp"
//#include "settings_utils.hpp"

extern "C"
{
    #include <psensor/psensor.h>
    #include <psensor/lmsensor.h>
    #include <psensor/hdd.h>
    #include <psensor/nvidia.h>
    #include <psensor/amd.h>
    #include <psensor/pgtop2.h>
    #include <psensor/pudisks2.h>
    #include <psensor/bcm2835.h>
    #include <psensor/psensor_json.h>
    #include <log_c/log.h>
}






void create_sensor_list()
{
    if(sensor_list_lmsensors)
        lmsensor_psensor_list_append(&sensors, 600);
    
    if(sensor_list_hddtemp)
        hddtemp_psensor_list_append(&sensors, 600);
    
    if(sensor_list_atasmart)
        atasmart_psensor_list_append(&sensors, 600);
    
    if(sensor_list_nvidia)
        nvidia_psensor_list_append(&sensors, 600);
    
    if(sensor_list_bcm2835)
        bcm2835_psensor_list_append(&sensors, 600);
    
    if(sensor_list_amd)
        amd_psensor_list_append(&sensors, 600);
    
    if(sensor_list_gtop)
        gtop2_psensor_list_append(&sensors, 600);
    
    if(sensor_list_udisks2)
        udisks2_psensor_list_append(&sensors, 600);
    
    sensor_list_created = true;
    log_info("Sensor list created");
}

void update_sensor_list(std::vector<ui_sensor>& sl)
{
    if(!sensor_list_created)
    {
        log_error("Sensor list not created");
        return;
    }

    log_info("Sensor update started");
    
    while(keep_sensor_update)
    {
        if(sensor_list_lmsensors)
            lmsensor_psensor_list_update(sensors);
        
        if(sensor_list_nvidia)
            nvidia_psensor_list_update(sensors);
        
        if(sensor_list_bcm2835)
            bcm2835_psensor_list_update(sensors);
        
        if(sensor_list_amd)
            amd_psensor_list_update(sensors);
        
        if(sensor_list_udisks2)
            udisks2_psensor_list_update(sensors);
        
        if(sensor_list_gtop)
            gtop2_psensor_list_update(sensors);
        
        if(sensor_list_atasmart)
            atasmart_psensor_list_update(sensors);
        
        if(sensor_list_hddtemp)
            hddtemp_psensor_list_update(sensors);

        std::vector<ui_sensor> temp_list;
        std::lock_guard<std::mutex> lock(sensors_mutex);
        if(sensors == nullptr)
        {
            log_error("No sensor list available. Check psensor providers");
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "No sensor list available.\nCheck psensor providers", NULL);
            exit(1);
        }
        for(int i = 0; sensors[i] != nullptr; i++)
        {
            temp_s = sensors[i];
            temp_list.emplace_back(
                temp_s->name,
                psensor_get_current_value(temp_s),
                temp_s->sess_lowest,
                temp_s->sess_highest,
                temp_s->type,
                graph_colors[i],
                true
            );
            //std::cout << "Stuff | id: " << temp_s->id << " | chip: " << temp_s->chip << std::endl;
        }

        {
            std::lock_guard<std::mutex> lock(sensor_list_mutex);
            sl = std::move(temp_list);
            sensor_count = sl.size();
            plot_update_interval += 0.1f;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(sensor_list_up_interv));
    }
}
