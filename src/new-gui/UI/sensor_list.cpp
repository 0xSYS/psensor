#include <vector>
#include <cstdlib>
#include <cstdio>
#include <thread>
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




//std::vector<psensor> sensors;
struct psensor **sensors = nullptr;


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
    std::cout << "Update\n";
    create_sensor_list();
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
					
		// Trying to get the sensor count but it's done in a weird way
        for(int i = 0; sensors[i] != nullptr; i++)
        {
            sensor_count = i;
        }
		
        for(int i = 0; sensors[i] != nullptr; i++)
        {
            struct psensor *s = sensors[i];
            printf("Count: %d\n", sensor_count);
            //printf("Sensor Index [%d]: Type: %u | Current Value : %.2f | Session High: %f | Session Low: %f | Name: %s | Chip: %s \n", i, s->type, psensor_get_current_value(*sensors), s->sess_highest, s->sess_lowest, s->name, s->chip);
            sl.emplace_back(
                s->name,
                //static_cast<int>(s->sess_lowest),
                //static_cast<int>(s->min),
                //static_cast<int>(s->max),
                s->sess_lowest,
                s->min,
                s->max,
                graph_colors[i],
                false
            );
        }
	    
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
