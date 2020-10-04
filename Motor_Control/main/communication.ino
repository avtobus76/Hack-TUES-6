#include "Common.h"

void init_communication()
{
  pinMode(CS, INPUT);
  pinMode(RESP, OUTPUT);
  digitalWrite(RESP, LOW);
  Wire.onReceive(receive);
  Wire.onRequest(transmit);
  Wire.begin(MC_ADDR);
}

void receive(int n)
{
  int pos = 0;
  while(Wire.available())
  {
    data_in[pos] = Wire.read();
    pos++;
  }
}

void transmit()
{
  Wire.write(data_out[0]);
  Wire.write(data_out[1]);
}

void transcieve()
{
  while(digitalRead(CS)!= HIGH);
  digitalWrite(RESP, HIGH);
  while(digitalRead(CS)!=LOW);
  digitalWrite(RESP, LOW);
}
