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
}

void update_sensor_list()
{
    create_sensor_list();
    while(1)
	{
	    lmsensor_psensor_list_update(sensors);
	    nvidia_psensor_list_update(sensors);
	    amd_psensor_list_update(sensors);
	    udisks2_psensor_list_update(sensors);
	    gtop2_psensor_list_update(sensors);
	    atasmart_psensor_list_update(sensors);
	    hddtemp_psensor_list_update(sensors);
		
        for(int i = 0; sensors[i] != NULL; i++)
        {
            struct psensor *s = sensors[i];
            printf("Sensor Index [%d]: | Name: %s | Chip: %s | Type: %u | Current Value : %.2f | Min Value: %f | Max Value: %f | Session High: %f | Session Low: %f\n", i, s->name, s->chip, s->type, psensor_get_current_value(*sensors), s->min, s->max, s->sess_highest, s->sess_lowest);
        }
        printf("\n");
   
	    
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	}
}
