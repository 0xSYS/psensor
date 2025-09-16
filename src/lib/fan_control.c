/*
 * Copyright (C) 2025 xsys061@gmail.com
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
 
 
 
/*
Todo:
- More accurate fan detection based on small and fast speed tests (This should only detect the physical fans)
- Functions for associating fans with thermal sensors / zones (Requires writing to file containing data serialized in messagepack)
*/




#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <errno.h>

#include <log_c/log.h>


#include "include/psensor/fan_control.h"
#include "include/psensor/pmod.h"





FILE * PWM_Enable_FD[MAX_FANS];
FILE * PWM_Fan_FD[MAX_FANS];
int last_pwm_value[MAX_FANS] = {0};



int isFanInput(const char *filename)
{
    if(strncmp(filename, "fan", 3) == 0 && isdigit(filename[3]) && strcmp(filename + 4, "_input") == 0)
    {
        return 1;
    }
    return 0;
}

// Search for pwm<digit> and pwm<digit>_enable files
void getPwmF(psensor_fan *list, const char *directory, int fan_number)
{
    char pwmFile[SMALL_BUF_SIZE];
    char pwmEnableFile[SMALL_BUF_SIZE];
    char pwmPath[SMALL_BUF_SIZE];
    char pwmEnablePath[SMALL_BUF_SIZE];

    snprintf(pwmFile, SMALL_BUF_SIZE, "pwm%d", fan_number);
    snprintf(pwmEnableFile, SMALL_BUF_SIZE, "pwm%d_enable", fan_number);

    snprintf(pwmPath, SMALL_BUF_SIZE, "%s/%s", directory, pwmFile);
    snprintf(pwmEnablePath, SMALL_BUF_SIZE, "%s/%s", directory, pwmEnableFile);

    if(access(pwmPath, F_OK) == 0)
    {
        list->pwmFiles[list->fanInputCount] = strdup(pwmPath);
    }
    else
    {
        list->pwmFiles[list->fanInputCount] = NULL;
    }

    if(access(pwmEnablePath, F_OK) == 0)
    {
        list->pwmEnableFiles[list->fanInputCount] = strdup(pwmEnablePath);
    }
    else
    {
        list->pwmEnableFiles[list->fanInputCount] = NULL;
    }
    list->fanInputCount++;
}


void psensor_fan_open(psensor_fan *f)
{
    char content[10];
    if(f == NULL)
    {
        log_error("Invalid fan control object");
        return;
    }
    
    for(int i = 0; i < f->fanInputCount; i++)
    {
        PWM_Enable_FD[i] = fopen(f->pwmEnableFiles[i], "r+");
        if(PWM_Enable_FD[i] == NULL)
            log_error("[PWM ENABLE] - Failed to open %s", f->pwmEnableFiles[i]);
        else
            log_debug("[PWM ENABLE] - Opened %s", f->pwmEnableFiles[i]);
    }
    
    for(int i = 0; i < f->fanInputCount; i++)
    {
        PWM_Fan_FD[i] = fopen(f->pwmFiles[i], "r+");
        if(PWM_Fan_FD[i] == NULL)
            log_error("[PWM] - Failed to open %s", f->pwmFiles[i]);
        else
            log_debug("[PWM] - Opened %s", f->pwmFiles[i]);
        
        // Also get the lat PWM value from sysfs as initial value
        
        if(PWM_Fan_FD[i] == NULL)
        {
            log_error("Failed to open %s for reading last pwm value! errorno: %s", f->pwmFiles[i], strerror(errno));
            return;
        }
        rewind(PWM_Fan_FD[i]);
        fgets(content, 10, PWM_Fan_FD[i]);
        last_pwm_value[i] = atoi(content);
    }
}

// The actual fan detection function
psensor_fan *psensor_detectFans()
{
    if(!pmod_check_loaded(module_table[1].name) && !pmod_check_loaded(module_table[2].name) && !pmod_check_loaded(module_table[3].name))
        return NULL;
    
    //else if(!pmod_check_loaded(required_modules[2], mods))
    //    return NULL;
    //
    //else if(!pmod_check_loaded(required_modules[3], mods))
    //    return NULL;
    
    
    DIR *dir = opendir("/sys/class/hwmon");
    struct dirent *entry;

    if(!dir)
    {
        log_error("Failed to open sysfs directory!");
        return NULL;
    }

    psensor_fan *list = malloc(sizeof(psensor_fan));
    list->pwmFiles = NULL;
    list->pwmEnableFiles = NULL;
    list->fanInputCount = 0;

    // Read all available directories
    while((entry = readdir(dir)) != NULL)
    {
        // Skip                    this                           and that
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        char full_path[SMALL_BUF_SIZE];
        snprintf(full_path, sizeof(full_path), "%s/%s", "/sys/class/hwmon", entry->d_name);
        
        struct stat statbuf;
        if(stat(full_path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
        {
            DIR *subdir = opendir(full_path);
            struct dirent *subdir_entry;
            
            // Read all subdirectories
            while((subdir_entry = readdir(subdir)) != NULL)
            {
                // Skip                          this and that again
                if(strcmp(subdir_entry->d_name, ".") == 0 || strcmp(subdir_entry->d_name, "..") == 0)
                    continue;
                
                // Check if there's any fan<digit>_input file existing in any of the
                // hwon directories
                if(isFanInput(subdir_entry->d_name))
                {
                    int fan_number = subdir_entry->d_name[3] - '0';
                    
                    list->pwmFiles = realloc(list->pwmFiles,(list->fanInputCount + 1) * sizeof(char *));
                    list->pwmEnableFiles = realloc(list->pwmEnableFiles, (list->fanInputCount + 1) * sizeof(char *));
                    
                    if(!list->pwmFiles || !list->pwmEnableFiles)
                    {
                        log_error("Failed to re-allocate memory for storing pwm files!");
                        exit(EXIT_FAILURE);
                    }
                    getPwmF(list, full_path, fan_number);
                }
            }
            closedir(subdir);
        }
    }

    closedir(dir);

    // Return NULL if no fans exist
    if(list->fanInputCount == 0)
    {
        free(list);
        return NULL;
    }

    return list;
}

int psensor_enable_fan_pwm(psensor_fan *f, int index, uint8_t v)
{
    if(PWM_Enable_FD[index] == NULL)
    {
        log_error("Failed to enable fan PWM at \"%s\"", f->pwmEnableFiles[index]);
        return 1;
    }

    if(v > 1)
        log_warn("Fan PWM enable value is larger than expected.");

    fprintf(PWM_Enable_FD[index], "%d", v);

    fclose(PWM_Enable_FD[index]);
    return 0;
}

int psensor_fan_set_pwm(psensor_fan *f, int index, uint8_t PWM)
{
    if(PWM_Fan_FD[index] == NULL)
    {
        log_error("Failed to write to %s", f->pwmFiles[index]);
        return 1;
    }
    
    if(PWM > 255)
    {
        log_error("Invalid PWM value: %d", PWM);
        return 1;
    }
    
    fprintf(PWM_Fan_FD[index], "%d", PWM);
    fflush(PWM_Fan_FD[index]);   // <--- important
    last_pwm_value[index] = PWM;
    
    return 0;
}

int psensor_get_last_pwm(psensor_fan *f, int index)
{
    // Get the last PWM value from the number array instead of reading from sysfs (which caused the slight decrementation of the PWM value in the GUI)
    int Pwm = last_pwm_value[index];
    return Pwm;
}

int psensor_test_fan(psensor_fan *f, int index)
{
    // Another dumb bug where the test decrements the previous PWM value stored in the GUI
    // Something I forgot that arrays start from 0 not 1 bruh moment
    for(int i = 0; i < 256; i++)
    {
        psensor_fan_set_pwm(f, index, i);
        log_debug("\033[38;5;33m[FAN TEST]\033[0m - Increment pwm: %d at ""\"\033[38;5;14m%s\033[0m\"", i, f->pwmFiles[index]);
        usleep(50 * 1000);
    }
    sleep(4);
    for(int i = 256; i >= 0; i--)
    {
        psensor_fan_set_pwm(f, index, i);
        log_debug("\033[38;5;33m[FAN TEST]\033[0m - Decrement pwm: %d at ""\"\033[38;5;14m%s\033[0m\"", i, f->pwmFiles[index]);
        usleep(50 * 1000);
    }
    return 0;
    sleep(4);
}


void psensor_fan_close(psensor_fan *f)
{
    log_trace("Closing fan controller");
    for(int i = 0; i < f->fanInputCount; i++)
    {
        fclose(PWM_Enable_FD[i]);
        log_debug("[PWM ENABLE] - Closed %s", f->pwmEnableFiles[i]);
    }
    
    for(int i = 0; f->pwmFiles[i] != NULL; i++)
    {
        fclose(PWM_Fan_FD[i]);
        log_debug("[PWM] - Closed %s", f->pwmFiles[i]);
    }
}