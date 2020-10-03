#include <Wire.h>
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

#define RECORDED 128
#define CMD_NUM 5
#define VR3_TGAM_ADDR 0x15

enum cmds{_FORWARD, _BACKWARD, _LEFT, _RIGHT, _STOP};
enum modules{VOICE_RECOGNITION_MODULE = 1};

enum input_data_t_vars{is_recording, module_enable, rec_number};
enum output_data_t_vars{voice_command};

char input_data_t[3] = {0, 0, 0};
char output_data_t = 0;

uint8_t stat[50];

void _receive(int);
void _transmit();
void calibrate(uint8_t);

VR myVR(2,3);

void setup() {
  Wire.begin(VR3_TGAM_ADDR);
  myVR.begin(9600); 
  Wire.onReceive(_receive);
  Wire.onRequest(_transmit);
}

void loop() {
  if(input_data_t[module_enable] == VOICE_RECOGNITION_MODULE)
  {
    if(input_data_t[is_recording])
    {
      calibrate(input_data_t[rec_number]);
      do{
        myVR.load(input_data_t[rec_number], stat);
      }
      while(stat[4] != 0);
      output_data_t[voice_command] |= RECORDED; 
    }

    int ret = myVR.recognize(stat, 50);
  
    if (ret>0)
    {    
      switch(stat[1])
      {
        case _FORWARD:
          output_data_t[voice_command] = _FORWARD;
          break;
  
        case _BACKWARD:
          output_data_t[voice_command] = _BACKWARD;
          break;
  
        case _LEFT:
          output_data_t[voice_command] = _LEFT;
          break;
  
        case _RIGHT:
          output_data_t[voice_command] = _RIGHT;
          break;
  
        case _STOP:
          output_data_t[voice_command] = _STOP;
          break;
      }
    }
  }  
}

void calibrate(uint8_t cmd)
{
  switch(cmd)
  {
    case _FORWARD:
      do {
        myVR.train(_FORWARD, stat);
      }
      while(stat[4] != 0);
      break;

    case _BACKWARD:
      do {
        myVR.train(_BACKWARD, stat);
      }
      while(stat[4] != 0);
      break;

    case _LEFT:
      do {
        myVR.train(_LEFT, stat);
      }
      while(stat[4] != 0);
      break;

    case _RIGHT:
      do {
        myVR.train(_RIGHT, stat);
      }
      while(stat[4] != 0);
      break;

    case _STOP:
      do {
        myVR.train(_STOP, stat);
      }
      while(stat[4] != 0);
      break;
  }
}

void _receive(int numBytes)
{
  int pos = 0;
  while(Wire.available())
  {
    input_data_t[pos] = Wire.read();
    pos ++;
  }
}

void _transmit()
{
  Wire.write(output_data_t, 2);
}
