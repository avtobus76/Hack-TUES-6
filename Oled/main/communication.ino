#include "Common.h"

void init_communication()
{
  Wire.begin();
  pinMode(VR3_CS, OUTPUT);
  pinMode(VR3_RESP, INPUT);
  pinMode(MC_CS, OUTPUT);
  pinMode(MC_RESP, INPUT);
  digitalWrite(VR3_CS, LOW);
  digitalWrite(MC_CS, LOW);
}

void transcieve()
{
  digitalWrite(VR3_CS, HIGH);
  while(digitalRead(VR3_RESP)!=HIGH);

  Wire.beginTransmission(VR3_ADDR);
  Wire.write(vr3_data_in, 2);
  
  Wire.endTransmission();

  Wire.requestFrom(VR3_ADDR, 1);

  while(Wire.available())
  {
    voice_commands = Wire.read();
  }

  digitalWrite(VR3_CS, LOW);  
      
  /*digitalWrite(MC_CS, HIGH);
  while(digitalRead(MC_RESP)!=HIGH);

  Wire.beginTransmission(MC_ADDR);
  for (int data = 0; data < 6; ++data)
    Wire.write(mc_data_in[data]);
  Wire.endTransmission(1);

  Wire.requestFrom(MC_ADDR, 2);

  int pos = 0;
  while(Wire.available())
  {
    mc_data_out[pos] = Wire.read();
    ++pos;
  }

  digitalWrite(MC_CS, LOW);*/   
}
/* 
 *  Input: 
 *    - Glasov: 
 *      > glasovi komandi
 *    - Motori: 
 *      > zapis na marshruti
 *      
 *  Output:
 *    - Glasov:
 *      > koi modul e vkluchen
 *      > koq komanda da se prezapishe
 *      > rejim na upravlenie
 *    - Motori:
 *      > rejim na upravlenie
 *      > skorosti 
 *    
 */
