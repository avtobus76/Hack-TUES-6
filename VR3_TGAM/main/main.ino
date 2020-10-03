#include "Common.h"

void setup() {
  _initCommunication();
  _initVR3();
  Serial.begin(9600);
}

void loop() {
  
  if(input_data_t[module_enable] == VOICE_RECOGNITION_MODULE)
  {
    Serial.println("Vlezna");
    if(input_data_t[is_recording])
      _record();

    int ret = myVR.recognize(stat, 50);
    if (ret>0)
    {    
      switch(stat[1])
      {
        case _FORWARD:
          output_data_t = _FORWARD;
          break;
  
        case _BACKWARD:
          output_data_t = _BACKWARD;
          break;
  
        case _LEFT:
          output_data_t = _LEFT;
          break;
  
        case _RIGHT:
          output_data_t = _RIGHT;
          break;
  
        case _STOP:
          output_data_t = _STOP;
          break;
      }
    }
  }
  Serial.println(output_data_t);
  Serial.print(input_data_t[is_recording]);
  Serial.println(input_data_t[module_enable]);
  transcieve();
}
