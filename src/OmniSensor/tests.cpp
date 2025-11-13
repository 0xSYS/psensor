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








#include "tests.h"
//#include "utils.hpp"
#include <stdlib.h>
#include <unistd.h>
//#include <stdio.h>
//#include <thread>
#include <iostream>

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
}

#include "UI/sensor_list.hpp"
#include "config_utils.hpp"

//struct psensor **sensors;

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

void read_sensor_list(std::vector<ui_sensor>& sensor)
{
    std::cout << "Read\n";
    while(1)
    {
        {
            std::lock_guard<std::mutex> lock(sensor_list_mutex);
            std::cout << "----------------------------------------------------------------------------\n";
            for(int i = 0; i < sensor_count; i++)
            {
                const auto& s = sensor[i];
                std::cout << "Name: " << s.name
                          << ", Current: " << s.current_value
                          << ", Min: " << s.min
                          << ", Max: " << s.max << "\n";
            }
        } // <-- Lock is released here before sleeping
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

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
	
/*
	std::vector<ui_sensor> test1;
	
	std::thread t(update_sensor_list, ref(test1));
	
	std::thread t2(read_sensor_list, ref(test1));
	t.join();   // main thread waits here
    t2.join();  // this line is never reached (optional)
*/

/*
    pconfig config_test;
    config_test.provider_gtop = 1;
    config_test.provider_lmsensors = 1;
    config_test.provider_udisks2 = 0;
    config_test.provider_atasmart = 1;
    config_test.provider_hddtemp = 0;
    config_test.provider_amd = 0;
    config_test.provider_nvidia = 1;
    
    config_test.window_w = 800;
    config_test.window_h = 600;
    
    config_test.graphics_platform = 2;
    
    config_test.save_ui_layout = 1;
    
    config_test.scroll_buffer_size = 1500;
    config_test.scroll_buffer_history = 28.5f;
    config_test.update_interval = 2;
    
    config_test.skip_module_loading = 0;
    config_test.emergency_cooling = 1;
    config_test.cooling_preset = "silent_efficiency.json";
    
    writeConfig(config_test);
*/

struct psensor **sensors = nullptr;
amd_psensor_list_append(&sensors, 600);
lmsensor_psensor_list_append(&sensors, 600);
hddtemp_psensor_list_append(&sensors, 600);
atasmart_psensor_list_append(&sensors, 600);
nvidia_psensor_list_append(&sensors, 600);
bcm2835_psensor_list_append(&sensors, 600);
gtop2_psensor_list_append(&sensors, 600);
udisks2_psensor_list_append(&sensors, 600);

psensor_list_free(sensors);


/*
    pconfig read_test;
    read_test = readConfig();
    
    printConfig(read_test);
*/
}
