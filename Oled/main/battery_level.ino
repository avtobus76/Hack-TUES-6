#include"Common.h"

void init_battery()
{
  pinMode(battery_level_pin, INPUT);
}

int get_battery_level()
{
  long battery_val = analogRead(battery_level_pin);
  battery_val = map(battery_val, 0, 1023, 0, 100);
  return battery_val;
}
