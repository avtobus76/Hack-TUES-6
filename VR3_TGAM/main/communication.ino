#include "Common.h"

void _initCommunication()
{
  Wire.onReceive(_receive);
  Wire.onRequest(_transmit);
  Wire.begin(VR3_TGAM_ADDR);
  pinMode(CS, INPUT);
  pinMode(RESPONSE, OUTPUT);
  digitalWrite(RESPONSE, LOW);
}

void _receive(int numBytes)
{
  int pos = 0;
  while(Wire.available())
  {
    input_data_t[pos] = Wire.read();
    pos ++;
  }
}

void _transmit()
{
  Wire.write(output_data_t);
}

void transcieve()
{
  while(digitalRead(CS) != HIGH);
  digitalWrite(RESPONSE, HIGH);
  while(digitalRead(CS) != LOW);
  digitalWrite(RESPONSE, LOW);
}
