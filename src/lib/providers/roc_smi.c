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
    
    // Test out what GPU parameters are available
    for(uint32_t i = 0; i < devices; i++)
    {
        uint16_t temp_activity;
        uint64_t temp_vram_used;
        uint64_t temp_vram_total;
        int64_t  temp_vram_temp;
        int64_t  temp_vram_clock;
        int64_t  temp_freq_clk;
        int64_t  temp_gpu_temp;
        int64_t  temp_fan_speed;
        uint64_t temp_power_avg;
        rsmi_frequencies_t temp_vram_freq;
        rsmi_frequencies_t temp_gpu_freq;
        
        
        ret = rsmi_dev_activity_avg_mm_get(i, &temp_activity);
        if(ret != RSMI_STATUS_SUCCESS)
            log_error("GPU Activity unavailable");
        
        ret = rsmi_dev_memory_usage_get(i, RSMI_MEM_TYPE_VRAM, &temp_vram_used);
        if(ret != RSMI_STATUS_SUCCESS)
            log_error("GPU VRAM Usage unavailable");
        
        ret = rsmi_dev_memory_total_get(i, RSMI_MEM_TYPE_VRAM, &temp_vram_total);
        if(ret != RSMI_STATUS_SUCCESS)
            log_error("GPU Total VRAM unavailable");
        
        ret = rsmi_dev_temp_metric_get(i, RSMI_TEMP_TYPE_MEMORY, RSMI_TEMP_CURRENT, &temp_vram_temp);
        if(ret != RSMI_STATUS_SUCCESS)
            log_error("GPU VRAM Temperature unavailable");
        
        ret = rsmi_dev_gpu_clk_freq_get(i, RSMI_CLK_TYPE_MEM, &temp_vram_freq);
        if(ret != RSMI_STATUS_SUCCESS)
            log_error("GPU Memory Clock Frequency unavailable");
        
        ret = rsmi_dev_gpu_clk_freq_get(i, RSMI_CLK_TYPE_SYS, &temp_gpu_freq);
        if(ret != RSMI_STATUS_SUCCESS)
            log_error("GPU System Clock Frequency unavailable");
        
        ret = rsmi_dev_temp_metric_get(i, RSMI_TEMP_TYPE_EDGE, RSMI_TEMP_CURRENT, &temp_gpu_temp);
        if(ret != RSMI_STATUS_SUCCESS)
            log_error("GPU Edge Temperature unavailable");
        
        ret = rsmi_dev_fan_speed_get(i, 0, &temp_fan_speed);
        if(ret != RSMI_STATUS_SUCCESS)
            log_error("GPU Fan Speed unavailable");
        
        ret = rsmi_dev_power_ave_get(i, 0, &temp_power_avg);
        if(ret != RSMI_STATUS_SUCCESS)
            log_error("GPU Power Average unavailable");
    }
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
		
		case 5:
		    sprintf(name, "GPU %d Temperature", id);
			sensor_type |= SENSOR_TYPE_GPU | SENSOR_TYPE_TEMP;
		break;
		
		case 6:
		    sprintf(name, "GPU %d Fan Speed", id);
			sensor_type |= SENSOR_TYPE_GPU | SENSOR_TYPE_PWM;
		break;
		
		case 7:
		    sprintf(name, "GPU %d Power avg", id);
			sensor_type |= SENSOR_TYPE_GPU | SENSOR_TYPE_GPU_POWER_AVG | SENSOR_TYPE_WATT;
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
        for(j = 0; j < 8; j++)
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
    int64_t gpu_temp[devices];
    int64_t gpu_fan_speed[devices];
    uint64_t gpu_power_avg[devices];

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
        if(ret == RSMI_STATUS_SUCCESS)
            gpu_freq_clk[i] = gfx_freq.current;
        else
            gpu_freq_clk[i] = 0;
        
        // GPU Temperature
        ret = rsmi_dev_temp_metric_get(i, RSMI_TEMP_TYPE_EDGE, RSMI_TEMP_CURRENT, &gpu_temp[i]);
        if(ret != RSMI_STATUS_SUCCESS)
            gpu_temp[i] = 0;
        
        // GPU Fan Speed
        ret = rsmi_dev_fan_speed_get(i, 0, &gpu_fan_speed[i]);
        if(ret != RSMI_STATUS_SUCCESS)
            gpu_fan_speed[i] = 0;
        
        // GPU Power Average
        ret = rsmi_dev_power_ave_get(i, 0, &gpu_power_avg[i]);
        if(ret != RSMI_STATUS_SUCCESS)
            gpu_power_avg[i] = 0;
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
        else if((s->type & SENSOR_TYPE_TEMP))
        {
            psensor_set_current_value(s, gpu_temp[id] / 1000.0);
        }
        else if((s->type & SENSOR_TYPE_PWM))
        {
            // GPU fan speed
            psensor_set_current_value(s, (double)gpu_fan_speed[id]);
        }
        else if((s->type & SENSOR_TYPE_GPU_POWER_AVG) && (s->type & SENSOR_TYPE_WATT))
        {
            // GPU power usage
            psensor_set_current_value(s, (double)gpu_power_avg[id]);
        }

        ss++;
    }
}

void roc_smi_cleanup(void)
{
    ret = rsmi_shut_down();
}