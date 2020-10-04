#include "Common.h"

void setup() {
  _InitMotors();
  init_brakes();
  init_joystick();
  init_bluetooth();
  init_ps();
  memory_init();
  init_communication();
  Serial.begin(9600);
} 

void loop() {
  read_bt_command();
  switch(bt_command)
  {
    case 'O':
    case 'o':
      bt_key_stat = bt_command;
      if(check_key_status(BT_KEY) == check_key_status(ONBOARD_KEY))
      {
        digitalWrite(power_supply_pin, HIGH);
        kolichka_on = 1;
      }
      else
      {
        digitalWrite(power_supply_pin, LOW);
        kolichka_on = 0;
      }
      bt_command = 0;
      break;

    case 'A':
      if(kolichka_on)
        data_out[ALARM] = 1;
      bt_command = 0;
      break;

    case 'a':
      if(kolichka_on)
        data_out[ALARM] = 0;
      bt_command = 0;
      break;

    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      if(kolichka_on)
      {
        recording = 1;
        data_out[SAVE_ROUTE] = 1;
        route_index = bt_command;
      }
      bt_command = 0;
      break;

    case 'S':
      if(kolichka_on)
      {
        recording = 0;
        data_out[SAVE_ROUTE] = 0;
      }
      bt_command = 0;
      break;
      
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      if(kolichka_on)
      {
        if(bt_command > 10)
        {
          route = download_route(bt_command-10);
          perform_route(*route, FORWARD, run_motors, invert_command);
        }
        else
        {
          route = download_route(bt_command-5);
          perform_route(*route, REVERSE, run_motors, invert_command);
        }        
      }
      bt_command = 0;
      break;
  }
  
  if(kolichka_on)
  {
    Serial.print(data_in[MODE]);
    Serial.print(data_in[GEAR]);
    Serial.print(data_in[BRAKES]);
    Serial.print(data_in[IN_MENU]);
    Serial.print(data_in[VR_COMMAND]);
    Serial.println(data_in[SAVE_ROUTE]);
    Serial.print(data_out[ALARM]);
    Serial.println(data_out[IS_RECORDING]);
    transcieve();
    
    char command = 0;
    
    switch(data_in[MODE])
    {
      case 1:
        if(!data_in[IN_MENU])
          command = get_joystick_position();
        break;
        
      case 2:
        command = data_in[VR_COMMAND];
        break;
    }
    
    switch(command)
    {
      case _FORWARD:
        settings._direction = command;
        settings._gear = data_in[GEAR];
        _move(settings);
        if(recording && data_in[MODE] == 1)
        {
          if(!saved)
          {
            dur = millis();
            com = command;
            saved = 1; 
          }
        }
        break;
        
      case _BACKWARD:
        settings._direction = command;
        settings._gear = data_in[GEAR];
        _move(settings);
        if(recording && data_in[MODE] == 1)
        {
          if(!saved)
          {
            dur = millis();
            com = command;
            saved = 1; 
          }
        }
        break;
        
      case _LEFT:
        settings._direction = command;
        settings._gear = data_in[GEAR];
        _move(settings);
        if(recording && data_in[MODE] == 1)
        {
          if(!saved)
          {
            dur = millis();
            com = command;
            saved = 1; 
          }
        }
        break;
        
      case _RIGHT:
        settings._direction = command;
        settings._gear = data_in[GEAR];
        _move(settings);
        if(recording && data_in[MODE] == 1)
        {
          if(!saved)
          {
            dur = millis();
            com = command;
            saved = 1; 
          }
        }
        break;
        
      case _STOP:
        settings._direction = command;
        settings._gear = data_in[GEAR];
        _move(settings);
        if(recording && data_in[MODE] == 1)
        {
          add_command(route, com, millis()-dur, data_in[GEAR]);
          saved = 0;
        }
        break;
    }
  }
}
