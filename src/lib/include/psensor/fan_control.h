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
 
#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H
 
 
 
#define MAX_FANS 256
#define MAX_HWMON_DIRS 9
#define SMALL_BUF_SIZE 128
 
 
#include <stdint.h>

#include "psensor.h"
 
 
 
 
typedef struct
{
    // Kind of poorly designed (A re-write will be made sometime...)
    char **pwmFiles;
    char **pwmEnableFiles;
    int fanInputCount;  // Number of fanX_input files
}psensor_fan;
 
 
 
 /*
 Opens both pwm_enable and pwm files 
 */
void psensor_fan_open(psensor_fan *f);
 
 
/*
Detect existing cooling fans addressed by the linux kernel inside hwmon directory
Returns a structure composed out of these parameters:
- fan.._input file count which represents a physical fan inside your computer.
- pwm_enable files which allow the fans to be controlled by writing a PWM value
- pwm files. Those files hold the PWM value assigned to them
- NULL if no fans could be detected
*/
psensor_fan *psensor_detectFans();
 
/*
Enable PWM fan control
Requires full path to pwm enable file and a value, usually 1 or 0
Returns 0 if the function is executed successfully if not it returns 1
Example: int fn_stat = psensor_enable_fan_pwm("/sys/class/hwmon/hwmon4/pwm1_enable", 1);
*/
int psensor_enable_fan_pwm(psensor_fan *f, int index, uint8_t v);
 
/*
Write a PWM value to the existing fan pwm files created by the kernel.
Requires full path to the hardware directory + a PWM value (0 - 255 only!)
Returns 0 if the function is executed successfully if not it returns 1
Example: int fn_stat = psensor_fan_set_pwm("/sys/class/hwmon/hwmon4/pwm1", 230);
*/
int psensor_fan_set_pwm(psensor_fan *f, int index, uint8_t PWM);
 
/*
Get the last pwm value from hwmon sysfs
Requires full path to pwm file
Returns 0 by default and if it fails, if not it returns the last value of a PWM file
Example int LastFanPwm = psensor_get_last_pwm("/sys/class/hwmon/hwmon4/pwm1");
*/
int psensor_get_last_pwm(psensor_fan *f, int index);
 
 
/*
Test the speed of a fan by slowly increasing and decreasing the PWM value
Requires the full path to the PWM file
Returns 0 if the function is executed successfully if not it returns 1
Example: int fn_stat = psensor_test_fan("/sys/class/hwmon/hwmon3/pwm1");
*/
int psensor_test_fan(psensor_fan *f, int index);


/*
Associate each fan with a thermal sensor
*/
int psensor_associate_fan(psensor_fan *f, struct psensor *s);
 
 
/*
Close the pwm and pwm_enable files after setting the fan speeds
*/
void psensor_fan_close(psensor_fan *f);
 
#endif