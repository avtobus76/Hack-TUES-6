#include "Common.h"

void init_bluetooth()
{
  Serial.begin(9600);
}

char read_bt_command()
{
  if(Serial.available())
    bt_command = Serial.read();
}
