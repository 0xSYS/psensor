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













#include <stdlib.h>
#include <string.h>

#include <rocm_smi/rocm_smi.h>
#include <log_c/log.h>
#include "roc_smi.h"




rsmi_status_t ret;
uint32_t devices;

void roc_smi_init()
{
    ret = rsmi_init(0);
    
    if(ret != RSMI_STATUS_SUCCESS)
    {
        log_error("Failed to initialize RSMI!");
        return;
    }
    else
        log_info("RSMI initialized successfully!");
    
    rsmi_num_monitor_devices(&devices);
}

static struct psensor *create_sensor(int id, int type, int values_len)
{
    char name[200];
	char *sid;
	int sensor_type;
	struct psensor *s;
	
	sensor_type = SENSOR_TYPE_ROC_SMI;
	
	
	switch(type)
	{
	    case 0:
			sprintf(name, "GPU %d Usage", id);
	        sensor_type |= SENSOR_TYPE_GPU | SENSOR_TYPE_PERCENT;
	    break;
					
		case 1:
		    sprintf(name, "GPU %d VRAM Usage", id);
			sensor_type |= SENSOR_TYPE_GPU | SENSOR_TYPE_GPU_VRAM;
		break;
		
		case 2:
		    sprintf(name, "GPU %d Clock", id);
			sensor_type |= SENSOR_TYPE_GPU | SENSOR_TYPE_FREQUENCY;
		break;
		
		case 3:
		     sprintf(name, "GPU %d VRAM Temperature", id);
			 sensor_type |= SENSOR_TYPE_GPU | SENSOR_TYPE_GPU_VRAM | SENSOR_TYPE_TEMP;
		break;
		
		case 4:
		    sprintf(name, "GPU %d VRAM Clock", id);
			sensor_type |= SENSOR_TYPE_GPU | SENSOR_TYPE_GPU_VRAM | SENSOR_TYPE_FREQUENCY;
		break;
	}
	
	sid = malloc(strlen("roc-smi") + 1 + strlen(name) + 1);
	sprintf(sid, "roc-smi %s", name);
	
	
	s = psensor_create(sid, strdup(name), strdup("AMD/ROC SMI GPU"), sensor_type, values_len);

	s->amd_id = id;
	
	return s;
}

void roc_smi_psensor_list_append(struct psensor ***sensors, int vl)
{
    int i, j, n;
	struct psensor *s;
	
	
	roc_smi_init();
	
	for(i = 0; i < devices; i++)
	{
        for(j = 0; j < 5; j++)
	    {
		    s = create_sensor(i, j, vl);
		    psensor_list_append(sensors, s);
	    }
	}
}

void roc_smi_psensor_list_update(struct psensor ** sensors)
{
    if(devices == 0)
        return;

    rsmi_status_t ret;
    struct psensor **ss, *s;

    uint16_t gpu_activity[devices];
    uint64_t gpu_vram_used[devices];
    uint64_t gpu_vram_total[devices];
    int64_t gpu_vram_temp[devices];
    int64_t gpu_vram_clock[devices];
    int64_t gpu_freq_clk[devices];

    // Fetch values once
    for(uint32_t i = 0; i < devices; i++)
    {
        // GPU usage
        ret = rsmi_dev_activity_avg_mm_get(i, &gpu_activity[i]);
        if(ret != RSMI_STATUS_SUCCESS)
            gpu_activity[i] = 0;

        // VRAM used
        ret = rsmi_dev_memory_usage_get(i, RSMI_MEM_TYPE_VRAM, &gpu_vram_used[i]);
        if(ret != RSMI_STATUS_SUCCESS)
            gpu_vram_used[i] = 0;

        // VRAM total
        ret = rsmi_dev_memory_total_get(i, RSMI_MEM_TYPE_VRAM, &gpu_vram_total[i]);
        if(ret != RSMI_STATUS_SUCCESS)
            gpu_vram_total[i] = 1; // set to 1 to avoid division by zero
        
        // VRAM Temperature
        ret = rsmi_dev_temp_metric_get(i, RSMI_TEMP_TYPE_MEMORY, RSMI_TEMP_CURRENT, &gpu_vram_temp[i]);
        if(ret != RSMI_STATUS_SUCCESS)
            gpu_vram_temp[i] = 0;
        
        
        // VRAM Clock
        rsmi_frequencies_t mem_freq;
        ret = rsmi_dev_gpu_clk_freq_get(i, RSMI_CLK_TYPE_MEM, &mem_freq);
        if(ret == RSMI_STATUS_SUCCESS)
            gpu_vram_clock[i] = mem_freq.current;
        else
            gpu_vram_clock[i] = 0;
        
        // GPU core clock
        rsmi_frequencies_t gfx_freq;
        ret = rsmi_dev_gpu_clk_freq_get(i, RSMI_CLK_TYPE_SYS, &gfx_freq);
        if (ret == RSMI_STATUS_SUCCESS)
            gpu_freq_clk[i] = gfx_freq.current;
        else
            gpu_freq_clk[i] = 0;
    }

    ss = sensors;
    while(*ss)
    {
        s = *ss;

        if(!(s->type & SENSOR_TYPE_ROC_SMI))
        {
            ss++;
            continue;
        }

        uint32_t id = s->amd_id;
        if(id >= devices)
        {
            ss++;
            continue;
        }
        
        if(s->type & SENSOR_TYPE_PERCENT)
        {
            psensor_set_current_value(s, gpu_activity[id]);
        }
        else if((s->type & SENSOR_TYPE_GPU_VRAM) && (s->type & SENSOR_TYPE_TEMP))
        {
            // VRAM temperature
            psensor_set_current_value(s, (double)gpu_vram_temp[id]);
        }
        else if((s->type & SENSOR_TYPE_GPU_VRAM) && (s->type & SENSOR_TYPE_FREQUENCY))
        {
            // VRAM clock
            psensor_set_current_value(s, (double)gpu_vram_clock[id]);
        }
        else if((s->type & SENSOR_TYPE_GPU_VRAM))
        {
            // VRAM usage
            double pct = (double)gpu_vram_used[id] / gpu_vram_total[id] * 100.0;
            psensor_set_current_value(s, pct);
        }
        else if((s->type & SENSOR_TYPE_GPU) && (s->type & SENSOR_TYPE_FREQUENCY))
        {
            // GPU core clock
            psensor_set_current_value(s, (double)gpu_freq_clk[id]);
        }

        ss++;
    }
}

void roc_smi_cleanup(void)
{
    ret = rsmi_shut_down();
}