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














#pragma once

#include <signal.h>

#define CMD_BUFFR_SZ 128


/*
Get the PID of the fancontroll script.
Returns pid (if successfull), -1 if something goes wrong.

Used to kill the fancontrol script from runing if manual fan control mode is enabled from the Fan Controller
*/
pid_t getFancontrolPID();

/*
Kill process based on the recevied PID
Returns 0 if the operation was sucessful if not it return -1
*/
int killPidProc(pid_t p);