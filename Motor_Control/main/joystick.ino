#include "Common.h"

void init_joystick()
{
  pinMode(joystick_x, INPUT);
  pinMode(joystick_y, INPUT);
}

char get_joystick_position()
{
  int vr_x = analogRead(joystick_x);
  int vr_y = analogRead(joystick_y);

  if(vr_x < 341)
    return _LEFT;
  else if(vr_x >= 341 && vr_x < 682)
  {
    if(vr_y < 341)
      return _BACKWARD;
    else if(vr_y >= 341 && vr_y < 682)
      return _STOP;
    else if(vr_y >= 682)
      return _FORWARD;
  }
  return _RIGHT;
}
