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