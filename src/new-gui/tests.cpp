#include "tests.h"
#include "utils.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <psensor/psensor.h>
#include <psensor/lmsensor.h>
#include <psensor/hdd.h>
#include <psensor/nvidia.h>
#include <psensor/amd.h>
#include <psensor/pgtop2.h>
#include <psensor/pudisks2.h>
#include <psensor/psensor_json.h>

struct psensor **sensors;

/*
void create_sensor_list()
{
    sensors = malloc(sizeof(struct psensor *));
    *sensors = NULL;
    
    lmsensor_psensor_list_append(&sensors, 600);
    hddtemp_psensor_list_append(&sensors, 600);
    atasmart_psensor_list_append(&sensors, 600);
    nvidia_psensor_list_append(&sensors, 600);
    amd_psensor_list_append(&sensors, 600);
    gtop2_psensor_list_append(&sensors, 600);
    udisks2_psensor_list_append(&sensors, 600);
}

*/

void test_stuff()
{
    /*
    create_sensor_list();
	int period;
	
	while(1)
	{
	    lmsensor_psensor_list_update(sensors);
	    nvidia_psensor_list_update(sensors);
	    amd_psensor_list_update(sensors);
	    udisks2_psensor_list_update(sensors);
	    gtop2_psensor_list_update(sensors);
	    atasmart_psensor_list_update(sensors);
	    hddtemp_psensor_list_update(sensors);
		
        for (int i = 0; sensors[i] != NULL; i++)
        {
            char * jsonified = sensor_to_json_string(sensors[i]);
            struct psensor *s = sensors[i];
            //printf("Sensor Index [%d]: | Name: %s | Chip: %s | Type: %u | Current Value : %.2f | Min Value: %f | Max Value: %f | Session High: %f | Session Low: %f\n", i, s->name, s->chip, s->type, psensor_get_current_value(*sensors), s->min, s->max, s->sess_highest, s->sess_lowest);
            printf("%s\n", jsonified);
        }
        printf("\n");

	    
		sleep(1);
	}
	*/
	
	// **The moment of realization that I had a bug in the core library...**
	//for(int i = 0; i < 256; i++)
	//    printf("Fan PWM %d\n", i);
	
	// No longer needed anymore, imgui already does this
	//Utils::open_url("https://github.com/0xSYS/NVi-PFA/blob/my-stuff/app/jni/src/Gui.cxx");
}