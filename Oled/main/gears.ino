#include "Common.h"

void _ShowGear(uint8_t* _gear)
{
  indicator.setAll(_gear);
  return; 
}

void gearUp()
{
  if(curr_gear == _THIRD)
  {
    return;
  }
  curr_gear++;
}

void gearDown()
{
  if(curr_gear == _NEUTRAL)
  {
    return;
  }
  curr_gear--;
}
