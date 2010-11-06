#include "ftd2xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sstream>

class Sensor
{
public:
  FT_HANDLE deviceHandle;
  char *Description[64];
  int notStopped;
  
  void PrintDeviceInfo();
  int run();
  void stop();
  int currentReading();
  
private:
  
};

// int str2int (const string &str) {
//   stringstream ss(str);
//   int num;
//   if((ss >> num).fail())
//   { 
//     //ERROR
//     return 0;
//   }
//   return num;
// }