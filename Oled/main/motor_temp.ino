#include "Common.h"

void init_motors_temp()
{
  sensors.begin();
}

int get_motor_temp(int sensor_idx)
{
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(sensor_idx);
}
