#include "Common.h"

void init_ps()
{
  pinMode(power_supply_pin, OUTPUT);
  pinMode(onboard_key_pin, INPUT);
}

int check_key_status(int key)
{
  switch(key)
  {
    case BT_KEY:
      if(bt_key_stat == 'o')
        return 0;
      else
        return 1;
      break;

    case ONBOARD_KEY:
      if(digitalRead(onboard_key_pin) == LOW)
        return 0;
      else
        return 1;
      break;
  }
  return -1;
}
