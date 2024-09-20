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
