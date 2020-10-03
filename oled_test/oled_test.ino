#include <EEPROM.h>
#include "OLED_Driver.h"
#include "OLED_GFX.h"
#include <SPI.h>
//#include <Wire.h>

#define button 3
OLED_GFX oled = OLED_GFX();

enum addrs{MANUAL_ADDR = 100, VOICE_ADDR, BRAIN_ADDR};

uint8_t* sp_1;
uint8_t* sp_2;


int dis = 0;
int manual=0;
int voice=0;
int brain=0;
int save_voice;
int light=0;


void _receive(int);
void _transmit();
void show_menu(int, int);
void modes(int);
void Lock();
void save_routes();
void managment(int);
void save_voice_commands(int);
void show_stats();

void draw_line(int start_x, int start_y, int dir)
{
  if (dir == 0)
  {
    for (int i = 0; i < 128; i ++)
      oled.Draw_Pixel(start_x, start_y++);
  }

  else
  {
    for(int i = 0; i < 128; i ++)
      oled.Draw_Pixel(start_x++, start_y);
  }  
}

void setup() {
  //Wire.begin();
  pinMode(oled_cs, OUTPUT);
  pinMode(oled_rst, OUTPUT);
  pinMode(oled_dc, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();
  Serial.begin(9600);
  manual = EEPROM.read(MANUAL_ADDR);
  voice = EEPROM.read(VOICE_ADDR);
  brain = EEPROM.read(BRAIN_ADDR);
  
  oled.Device_Init();
  oled.Clear_Screen();
  oled.Set_Color(RED);
  oled.print_String(29, 54, (const uint8_t*)"AVTOBUS 76", FONT_8X16);
  delay(2500);
  oled.print_String(12, 54, (const uint8_t*)"Smart Kolichka", FONT_8X16);
  delay(5000);
  oled.Clear_Screen();
}

void loop() 
{
  int i=0;
  switch(dis)
  {
    case 0:
    {
      show_stats();
      break;
    }
    case 1:
    {
      oled.Clear_Screen();
      i=0;
      while(i<5)
      {
        show_menu(i);
        if(i<5)
        {
          if((int)analogRead(A2) == 0)
          {
            i++;
            delay(100);
          }
          if(i>0)
          {
            if((int)analogRead(A2) == 1023)
            {
              i--;
              delay(100);
            }
          }
        }
        if(i==1 && digitalRead(button)==LOW)
        {
          dis=2;
          break;
        }
        else if(i==4 && digitalRead(button)==LOW)
        {
          dis=0;
          break;
        }
        else if(i==0 && digitalRead(button)==LOW)
        {
          dis = 3;
          break;
        }
        else if(i==3 && digitalRead(button)==LOW)
        {
          dis = 4;
          break;
        }
        else if(i==2 && digitalRead(button)==LOW)
        {
          dis=5;
          break;
        }
      }
      break;
    }
    case 2:
    {
      i=0;
      oled.Clear_Screen();
      while(i<4)
      {
        modes(i);
        if(i<4)
        {
          if((int)analogRead(A2) == 0)
          {
            i++;
            delay(100);
          }
          if(i>0)
          {
            if((int)analogRead(A2) == 1023)
            {
              i--;
              delay(100);
            }
          }
        }
        if(i==0 &&digitalRead(button)==LOW)
        {
          if(manual==1)
          {
            manual=0;
            EEPROM.write(MANUAL_ADDR, manual);
          }
          else
          {
            manual=1;
            EEPROM.write(MANUAL_ADDR, manual);
          }
        }
        if(i==1 && digitalRead(button)==LOW)
        {
          if(voice==1)
          {
            voice=0;
            EEPROM.write(VOICE_ADDR, voice);
          }
          else
          {
            voice=1;
            EEPROM.write(VOICE_ADDR, voice);
          }
        }
        if(i==2 && digitalRead(button)==LOW)
        {
          if(brain==1)
          {
            brain=0;
            EEPROM.write(BRAIN_ADDR, brain);
          }
          else
          {
            brain=1;
            EEPROM.write(BRAIN_ADDR, brain);
          }
        }
        if(i==3 && digitalRead(button)==LOW)
        {
          dis=1;
          break;
        }
      }
      break;
    }
    case 3:
    {
      oled.Clear_Screen();
      Lock();
      break;
    }
    case 4:
    {
      oled.Clear_Screen();
      save_routes();
      dis=1;
      break;
    }
    case 5:
    {
      int count=0;
      oled.Clear_Screen();
      while(count<3)
      {
        managment(count);
        if(count<3)
        {
          if((int)analogRead(A2) == 0)
          {
            count++;
            delay(100);
          }
          if(count>0)
          {
            if((int)analogRead(A2) == 1023)
            {
              count--;
              delay(100);
            }
          }
        }
        if(count==0 && digitalRead(button)==LOW)
        {
          dis=6;
          break;
        }
        if(count==1 && digitalRead(button)==LOW)
        {
          if(light == 0)
          {
            light=1;
            
          }
          else if(light == 1)
          {
            light=0;
          }
        }
        if(count==2 && digitalRead(button)==LOW)
        {
          dis=1;
          break;
        }
      }
      break;
    }
    case 6:
    {
      int count=0;
      oled.Clear_Screen();
      while(count<6)
      {
        save_voice_commands(count);
        if(count<6)
        {
          if((int)analogRead(A2) == 0)
          {
            count++;
            delay(100);
          }
          if(count>0)
          {
            if((int)analogRead(A2) == 1023)
            {
              count--;
              delay(100);
            }
          }
        }
        if(count==0 && digitalRead(button)==LOW)
        {
          save_voice=4;
          Serial.print(save_voice);
        }
        if(count==1 && digitalRead(button)==LOW)
        {
          save_voice=3;
          Serial.print(save_voice);
        }
        if(count==2 && digitalRead(button)==LOW)
        {
          save_voice=2;
          Serial.print(save_voice);
        }
        if(count==3 && digitalRead(button)==LOW)
        {
          save_voice=1;
          Serial.print(save_voice);
        }
        if(count==4 && digitalRead(button)==LOW)
        {
          save_voice=0;
          Serial.print(save_voice);
        }
        if(count==5 && digitalRead(button)==LOW)
        {
          dis=5;
          break;
        }
      }
      break;
    }
  }
}



void show_menu(int idx)
{
  oled.Set_Color(BLUE);
  oled.print_String(47, 5, (const uint8_t*)"MENU", FONT_8X16);
  draw_line(0, 23, 1);
  oled.Set_Color(WHITE);
  if(idx == 0)
  {
    oled.Set_Color(GREEN);
  }
  oled.print_String(47, 38, (const uint8_t*)"Lock", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 1)
    oled.Set_Color(GREEN);
  oled.print_String(45, 53, (const uint8_t*)"Modes", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 2)
    oled.Set_Color(GREEN);
  oled.print_String(27, 68, (const uint8_t*)"Managment", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 3)
    oled.Set_Color(GREEN);
  oled.print_String(21, 83, (const uint8_t*)"Save Routes", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 4)
    oled.Set_Color(GREEN);
  oled.print_String(47, 98, (const uint8_t*)"Stats", FONT_8X16);
  oled.Set_Color(WHITE);
}



void modes(int idx)
{
  oled.Set_Color(BLUE);
  oled.print_String(47, 5, (const uint8_t*)"MODES", FONT_8X16);
  draw_line(0, 23, 1);
  if(manual==1)
  {
    oled.Set_Color(GREEN);
  }
  else
  {
    oled.Set_Color(RED);
  }
  if(idx == 0)
  {
    oled.Set_Color(WHITE);
  }
  oled.print_String(44, 38, (const uint8_t*)"Manual", FONT_8X16);
  if(voice==1)
  {
    oled.Set_Color(GREEN);
  }
  else
  {
    oled.Set_Color(RED);
  }
  if(idx == 1)
    oled.Set_Color(WHITE);
  oled.print_String(47, 53, (const uint8_t*)"Voice", FONT_8X16);
  if(brain==1)
  {
    oled.Set_Color(GREEN);
  }
  else
  {
    oled.Set_Color(RED);
  }
  if(idx == 2)
    oled.Set_Color(WHITE);
  oled.print_String(35, 68, (const uint8_t*)"Brainwave", FONT_8X16);
  oled.Set_Color(BLUE);
  if(idx == 3)
    oled.Set_Color(WHITE);
  oled.print_String(50, 83, (const uint8_t*)"BACK", FONT_8X16);  
  oled.Set_Color(WHITE);
}



void Lock()
{
  oled.Set_Color(WHITE);
  oled.print_String(32, 30, (const uint8_t*)"Enter the", FONT_8X16);
  oled.print_String(35, 45, (const uint8_t*)"joystick", FONT_8X16);
  oled.print_String(26, 60, (const uint8_t*)"combination:", FONT_8X16);
  while(true)
  {
    if((int)analogRead(A2) > 1000)
    {
      Serial.println((int)analogRead(A2));
      delay(600);
      if((int)analogRead(A1) < 50)
      {
        Serial.println((int)analogRead(A1));
        dis=1;
        break;
      }
    }
  }
}



void save_routes()
{
  return;
}



void managment(int idx)
{
  oled.Set_Color(BLUE);
  oled.print_String(32, 5, (const uint8_t*)"Managment", FONT_8X16);
  draw_line(0, 23, 1);
  oled.Set_Color(WHITE);
  if(idx == 0)
  {
    oled.Set_Color(GREEN);
  }
  oled.print_String(14, 25, (const uint8_t*)"Voice_commands", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 1)
    oled.Set_Color(GREEN);
  oled.print_String(29, 40, (const uint8_t*)"Flashlight", FONT_8X16);
  oled.Set_Color(BLUE);
  if(idx==2)
    oled.Set_Color(GREEN);
  oled.print_String(47, 55, (const uint8_t*)"Back", FONT_8X16);
  oled.Set_Color(WHITE);
}



void save_voice_commands(int idx)
{
  oled.Set_Color(BLUE);
  oled.print_String(44, 5, (const uint8_t*)"Voice", FONT_8X16);
  draw_line(0, 23, 1);
  oled.Set_Color(WHITE);
  if(idx == 0)
  {
    oled.Set_Color(GREEN);
  }
  oled.print_String(47, 25, (const uint8_t*)"Stop", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 1)
    oled.Set_Color(GREEN);
  oled.print_String(44, 40, (const uint8_t*)"Right", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 2)
    oled.Set_Color(GREEN);
  oled.print_String(47, 55, (const uint8_t*)"Left", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 3)
    oled.Set_Color(GREEN);
  oled.print_String(32, 70, (const uint8_t*)"Backward", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 4)
    oled.Set_Color(GREEN);
  oled.print_String(35, 85, (const uint8_t*)"Forward", FONT_8X16);
  oled.Set_Color(BLUE);
  if(idx == 5)
    oled.Set_Color(GREEN);
  oled.print_String(47, 100, (const uint8_t*)"Back", FONT_8X16);
  oled.Set_Color(WHITE);
}



void show_stats()
{
  oled.Set_Color(BLUE);
  oled.print_String(47, 5, (const uint8_t*)"STATS", FONT_8X16);
  draw_line(0, 23, 1);

  oled.Set_Color(WHITE);
  oled.print_String(0, 38, (const uint8_t*)"Speed:  ", FONT_8X16);
  oled.print_String(0, 53, (const uint8_t*)"Distance:   0 km", FONT_8X16);
  oled.print_String(0, 68, (const uint8_t*)"Battery:    100%", FONT_8X16);
  oled.print_String(0, 83, (const uint8_t*)"Humidity:   100%", FONT_8X16);
  oled.print_String(0, 98, (const uint8_t*)"Air Temp:   25 C", FONT_8X16);
  
  for(int i = 0; i < 5; i ++)
  {
    for(int j = 0; j < 5; j ++)
    {
      if(digitalRead(button) == LOW)
      {
        dis=1;
        break;
      }
      *sp_1 = (i+'0');
      oled.print_String(63, 38, sp_1, FONT_8X16);
      oled.print_String(70, 38, ".", FONT_8X16);
      *sp_1 = (j+'0');
      oled.print_String(78, 38, sp_1, FONT_8X16);
      oled.print_String(88, 38, " km/h", FONT_8X16);
      delay(100);
    }
  }
}

/*void _receive(int)
{
  while(Wire.available())
  {
    Wire.read();
  }
}

void _transmit()
{
  
}*/
