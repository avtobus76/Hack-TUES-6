#include "Common.h"

void init_brakes()
{
  pinMode(brakes_pin, OUTPUT);
}

void engage_brakes()
{
  digitalWrite(brakes_pin, HIGH);
}

void disengage_brakes()
{
  digitalWrite(brakes_pin, LOW);
}
