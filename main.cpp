#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "ftd2xx.h"
#include "lib/sensor.h"

Sensor sensor;

void quit(int param)
{
  if(!sensor.notStopped)
  {
    sensor.stop();
  }
  exit(1);
}

int main (int argc, char const *argv[])
{
  signal(SIGINT, quit);		// trap ctrl-c call quit fn 
  
  printf("Searching for device...\n");
  
  char const *deviceSerial = (char *)"A8007ZmY";
  
  FT_HANDLE ftHandle;
  
  while (!sensor.deviceHandle)
  {
    FT_STATUS ftStatus;
    
    ftStatus = FT_OpenEx((char *)deviceSerial, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);
    
    if (ftStatus == FT_OK) {
      sensor.deviceHandle = ftHandle;      
    }
    
    sleep(0.5);
  }
  
  sensor.PrintDeviceInfo();
  
  int deviceStatus = sensor.run();
  
  if(deviceStatus == 1)
  {
    printf("==> ERROR: Failed to run, you probably don't have a device handle yet\n");
  }
  
  return 0;
}