#include "sensor.h";

void Sensor::PrintDeviceInfo(){
  int ftStatus;
  FT_DEVICE ftDevice;
  DWORD deviceID;
  char *SerialNumber[16]; 
    
  ftStatus = FT_GetDeviceInfo(deviceHandle, &ftDevice, &deviceID, (char *)SerialNumber, (char *)Description, NULL);
  
  if (ftStatus == FT_OK) {
    printf("==> Connected to %s\n", (char*)Description);
                
    if (ftDevice == FT_DEVICE_232R){ // device is FT232R
    }else if (ftDevice == FT_DEVICE_2232C){ // device is FT2232C/L/D
    }else if (ftDevice == FT_DEVICE_BM){ // device is FTU232BM
    }else if (ftDevice == FT_DEVICE_AM){ // device is FT8U232AM
    }else{
      // unknown device (this should not happen!)
      // deviceID contains encoded device ID
      // SerialNumber, Description contain 0-terminated strings 
    }
  }else {
    printf("==> ERROR: Failed to get device details");
  }
}

int Sensor::run()
{
  if(!deviceHandle)
  {
    return 1;
  }
  
  int ftStatus;
  if (deviceHandle != NULL) {

    ftStatus = FT_ResetDevice(deviceHandle);
    if (ftStatus != FT_OK) {
      printf("Unable to reset device\n");
    }

    ftStatus = FT_SetBaudRate(deviceHandle, 9600);
    if (ftStatus != FT_OK) {
      printf("Unable to set baud rate\n");
    }

    ftStatus = FT_SetDtr(deviceHandle);
    if (ftStatus != FT_OK) {
      printf("Unable to set DTR\n");
    }

    ftStatus = FT_SetRts(deviceHandle);
    if (ftStatus != FT_OK) {
      printf("Unable to set RTS\n");
    }

    ftStatus = FT_SetFlowControl(deviceHandle, FT_FLOW_RTS_CTS, 0, 0);
    if (ftStatus != FT_OK) {
      printf("Unable to set flow control\n");
    }

    ftStatus = FT_SetDataCharacteristics(deviceHandle, 8, 1, 0);
    if (ftStatus != FT_OK) {
      printf("Unable to set data characteristics\n");
    }

    printf("All good, starting read\n");
  }
  
  while(!notStopped) {
    int range;
    range = Sensor::currentReading();
    Sensor::processRange(range);
    
    sleep(0.1);
  }
  
  return 0;
}

int Sensor::currentReading()
{
  int     ftStatus, j;
  DWORD   dwRxSize = 0;
  DWORD   dwBytesRead;
  char    *pcBufRead = NULL;
  char    *respStr = NULL;
  
  dwRxSize = 0;
  while ((dwRxSize < 6) && (ftStatus == FT_OK)) {
    ftStatus = FT_GetQueueStatus(deviceHandle, &dwRxSize);
  }
  
  if(ftStatus == FT_OK) {
    // Allocate memory
    pcBufRead = (char*)realloc(pcBufRead, dwRxSize);
    // Fill memory with blanks
    memset(pcBufRead, 0xFF, dwRxSize);
      
    if((ftStatus = FT_Read(deviceHandle, pcBufRead, dwRxSize, &dwBytesRead)) != FT_OK) {
      printf("Unable to read from device\n");
      return 0;
    }else {
      int range = 0;
      
      std::stringstream bufferToStr;
      std::string s;
      bufferToStr << pcBufRead;
      bufferToStr >> s;
      
      std::stringstream stringToInt;
      stringToInt << s.substr(1,4);
      stringToInt >> range;
      
      return range;
    }
  }
}

void Sensor::processRange(int range)
{
  
  if(range < 70)
  {
    printf("Taking photo\n");
  }
}

void Sensor::stop()
{
  printf("\n\n==> Stopping...\n");
  
  notStopped = 1;
  
  if(deviceHandle != NULL) {
    FT_Close(deviceHandle);
    deviceHandle = NULL;
    printf("==> Closed connection\n");
  }
}