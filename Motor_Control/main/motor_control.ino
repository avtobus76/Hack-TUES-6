#include "Common.h"

void _InitMotors() 
{
  InitTimersSafe();
  while(!SetPinFrequencySafe(left_motor, 62500));
  while(!SetPinFrequencySafe(right_motor, 62500));
  return;
}
void run_motors(unsigned char com, unsigned long dur, unsigned int gear)
{
  settings._direction = com2dir(com);
  settings._gear = gear;
  _move(settings);
  delay(dur);
}

void _move(struct settings s)
{
  uint8_t control_register = 0;
  
  switch(s._direction)
  {
    case _FORWARD:
      switch(s._gear)
      {
        case _PARK:
          _SetSpeed(left_motor, 0);
          _SetSpeed(right_motor, 0);
          engage_brakes();
          control_register = 0;
          control_register |= (1 << SD1) | (1 << SD2);
          _UpdateControlReg(&control_register);
          break;
          
        case _NEUTRAL:
          _SetSpeed(left_motor, 0);
          _SetSpeed(right_motor, 0);
          disengage_brakes();
          control_register = 0;
          control_register |= (1 << SD1) | (1 << SD2);
          _UpdateControlReg(&control_register);
          break;
          
        case _FIRST:
          disengage_brakes();
          control_register = 0;
          control_register |= (1 << LIN1)|(1 << LIN3)|(1 << PWM_SELECT1)|(1 << PWM_SELECT2);
          //Serial.println(control_register);
          _UpdateControlReg(&control_register);
          _SetSpeed(left_motor, 72);
          _SetSpeed(right_motor, 72);
          break;
          
        case _SECOND:
          disengage_brakes();
          control_register = 0;
          control_register |= (1 << LIN1)|(1 << LIN3)|(1 << PWM_SELECT1)|(1 << PWM_SELECT2);
          _UpdateControlReg(&control_register);
          _SetSpeed(left_motor, 144);
          _SetSpeed(right_motor, 144);
          break;
          
        case _THIRD:
          disengage_brakes();
          control_register = 0;
          control_register |= (1 << LIN1)|(1 << LIN3)|(1 << PWM_SELECT1)|(1 << PWM_SELECT2);
          _UpdateControlReg(&control_register);
          _SetSpeed(left_motor, 216);
          _SetSpeed(right_motor, 216);
          break;
      }
      break;
      
    case _BACKWARD:
      switch(s._gear)
      {
        case _PARK:
          _SetSpeed(left_motor, 0);
          _SetSpeed(right_motor, 0);
          engage_brakes();
          control_register = 0;
          control_register |= (1 << SD1) | (1 << SD2);
          _UpdateControlReg(&control_register);
          break;
          
        case _NEUTRAL:
          _SetSpeed(left_motor, 0);
          _SetSpeed(right_motor, 0);
          disengage_brakes();
          control_register = 0;
          control_register |= (1 << SD1) | (1 << SD2);
          _UpdateControlReg(&control_register);
          break;
          
        case _FIRST:
          disengage_brakes();
          control_register = 0;
          control_register |= (1 << LIN2)|(1 << LIN4);
          _UpdateControlReg(&control_register);
          _SetSpeed(left_motor, 72);
          _SetSpeed(right_motor, 72);
          break;
          
        case _SECOND:
          disengage_brakes();
          control_register = 0;
          control_register |= (1 << LIN2)|(1 << LIN4);
          _UpdateControlReg(&control_register);
          _SetSpeed(left_motor, 144);
          _SetSpeed(right_motor, 144);
          break;
          
        case _THIRD:
          disengage_brakes();
          control_register = 0;
          control_register |= (1 << LIN2)|(1 << LIN4);
          _UpdateControlReg(&control_register);
          _SetSpeed(left_motor, 216);
          _SetSpeed(right_motor, 216);
          break;
      }
      break;
      
    case _LEFT:
      switch(s._gear)
      {
        case _PARK:
          _SetSpeed(left_motor, 0);
          _SetSpeed(right_motor, 0);
          engage_brakes();
          control_register = 0;
          control_register |= (1 << SD1) | (1 << SD2);
          _UpdateControlReg(&control_register);
          break;
          
        case _NEUTRAL:
          disengage_brakes();
          control_register = 0;
          control_register |= (1 << SD1) | (1 << SD2);
          _UpdateControlReg(&control_register);
          break;
          
        case _FIRST:
          disengage_brakes();
          control_register = 0;
          control_register |= (1 << LIN1)|(1 << LIN4)|(1 << PWM_SELECT1);
          _UpdateControlReg(&control_register);
          _SetSpeed(left_motor, 72);
          _SetSpeed(right_motor, 72);
          break;
          
        case _SECOND:
          disengage_brakes();
          control_register = 0;
          control_register |= (1 << LIN1)|(1 << LIN4)|(1 << PWM_SELECT1);
          _UpdateControlReg(&control_register);
          _SetSpeed(left_motor, 144);
          _SetSpeed(right_motor, 144);
          break;
          
        case _THIRD:
          disengage_brakes();
          control_register = 0;
          control_register |= (1 << LIN1)|(1 << LIN4)|(1 << PWM_SELECT1);
          _UpdateControlReg(&control_register);
          _SetSpeed(left_motor, 216);
          _SetSpeed(right_motor, 216);
          break;
      }
      break;
      
    case _RIGHT:
      switch(s._gear)
      {
        case _PARK:
          _SetSpeed(left_motor, 0);
          _SetSpeed(right_motor, 0);
          engage_brakes();
          control_register = 0;
          control_register |= (1 << SD1) | (1 << SD2);
          _UpdateControlReg(&control_register);
          break;
          
        case _NEUTRAL:
          _SetSpeed(left_motor, 0);
          _SetSpeed(right_motor, 0);
          disengage_brakes();
          control_register = 0;
          control_register |= (1 << SD1) | (1 << SD2);
          _UpdateControlReg(&control_register);
          break;
          
        case _FIRST:
          disengage_brakes();
          control_register = 0;
          control_register |= (1 << LIN2)|(1 << LIN3)|(1 << PWM_SELECT2);
          _UpdateControlReg(&control_register);
          _SetSpeed(left_motor, 72);
          _SetSpeed(right_motor, 72);
          break;
          
        case _SECOND:
          disengage_brakes();
          control_register = 0;
          control_register |= (1 << LIN2)|(1 << LIN3)|(1 << PWM_SELECT2);
          _UpdateControlReg(&control_register);
          _SetSpeed(left_motor, 144);
          _SetSpeed(right_motor, 144);
          break;
          
        case _THIRD:
          disengage_brakes();
          control_register = 0;
          control_register |= (1 << LIN2)|(1 << LIN3)|(1 << PWM_SELECT2);
          _UpdateControlReg(&control_register);
          _SetSpeed(left_motor, 216);
          _SetSpeed(right_motor, 216);
          break;
      }
      break;

    case _STOP:
      _SetSpeed(left_motor, 0);
      _SetSpeed(right_motor, 0);
      control_register = 0;
      control_register |= (1 << SD1) | (1 << SD2);
      _UpdateControlReg(&control_register);
      break;
  }
}

void _UpdateControlReg(uint8_t* control_byte)
{
  control_reg.setAll(control_byte);
  return;
}

void _SetSpeed(int _motor, uint8_t _speed)
{
  pwmWrite(_motor, _speed);
  return;
}

char dir2com(int dir)
{
  switch (dir)
  {
    case _FORWARD:
      return 'F';
    break;
    case _BACKWARD:
      return 'B';
    break;
    case _LEFT:
      return 'L';
    break;
    case _RIGHT:
      return 'R';
    break;
   }
   return 'S';
}

int com2dir(char com)
{
 switch (com)
  {
    case 'F':
      return _FORWARD;
    break;
    case 'B':
      return _BACKWARD;
    break;
    case 'L':
      return _LEFT;
    break;
    case 'R':
      return _RIGHT;
    break;
   }
   return _STOP;
}
