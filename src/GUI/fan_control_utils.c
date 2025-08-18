/*
 * Copyright (C) 2024 xsys061@gmail.com
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














#include "fan_control_utils.h"

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include <psensor/plog.h>




pid_t getFancontrolPID()
{
    char tempBuffr[CMD_BUFFR_SZ];
    FILE * fpipe;
    pid_t fanControlPid = -1;

    //Execute pgrep command to determine the pid of the fancontroll script
    fpipe = popen("pgrep -f fancontrol", "r");

    //Check if things go ok
    if(fpipe == NULL)
    {
        log_err("Failed to check for fancontrol script!");
        perror("popen");
        return -1;
    }

    //If no problems occured read the pid from stdout and convert it to an int and return it
    if(fgets(tempBuffr, CMD_BUFFR_SZ, fpipe) != NULL)
        fanControlPid = (pid_t)atoi(tempBuffr);

    pclose(fpipe);

  return fanControlPid;
}

int killPidProc(pid_t p)
{
    //Check if the pid is valid
    if(p <= 0)
    {
        log_err("Invalid pid: %d", p);
        return -1;
    }
    else
    {
        //If valid kill it
        if(kill(p, SIGTERM) == 0)
        {
            log_info("fancontrol script canceled.");
            return 0;
        }
        else
        {
            //If not throw error and return -1
            log_err("Failed to kill fancontrol script!");
            perror("kill");
            return -1;
        }
    }
  return 0;
}
