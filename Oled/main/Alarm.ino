#include "Common.h"

void init_alarm()
{
  pinMode(alarm_pin, OUTPUT);
}
 
void engage_alarm()
{
  for(int i=0; i<5000; i++)
  {
    tone(alarm_pin, i, 10);
  }
  for(int i=5000; i>=0; i--)
  {
    tone(alarm_pin, i, 10);
  }
}

void disengage_alarm()
{
  noTone(alarm_pin);
}

void engage_horn()
{ 
  tone(alarm_pin, 600);  
}

void disengage_horn()
{
  noTone(alarm_pin);
}
