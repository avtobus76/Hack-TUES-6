#include "Common.h"

void _initButtons()
{
  pinMode(horn_button, INPUT);
  pinMode(park_button, INPUT);
  pinMode(gearUp_button, INPUT);
  pinMode(gearDown_button, INPUT);
}

void _getHornButtonState()
{
  if(digitalRead(horn_button) == HIGH)
    horn_button_state = 1;

  if(digitalRead(horn_button) == LOW)
    horn_button_state = 0;
}

void _getParkButtonState()
{
  if(digitalRead(park_button) == HIGH)
    park_button_state = 1;

  if(digitalRead(park_button) == LOW)
    park_button_state = 0;
}

void _getGearUpButtonState()
{
  if(digitalRead(gearUp_button) == HIGH && gear_up_button_state == 0)
  {
    gear_up_button_state = 1;
  }
    

  if(digitalRead(gearUp_button) == LOW && gear_up_button_state == 1)
  {
    gear_up_button_state = 0;  
    do_ones_up = 0;
  }
}

void _getGearDownButtonState()
{
  if(digitalRead(gearDown_button) == HIGH && gear_down_button_state == 0)
    gear_down_button_state = 1;    

  if(digitalRead(gearDown_button) == LOW && gear_down_button_state == 1)
  {
    gear_down_button_state = 0;
    do_ones_down = 0;
  }
}
