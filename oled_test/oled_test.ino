#include "OLED_Driver.h"
#include "OLED_GFX.h"
#include <SPI.h>

#define button 3
OLED_GFX oled = OLED_GFX();

uint8_t* sp_1;
uint8_t* sp_2;

int dis = 0;
int i=0;

void show_stats();
void Lock();
void show_menu(int, int);
void modes(int);
void manual_control();
void voice_control();
void brainwave_control();

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
  Serial.print(dis);
  switch(dis)
  {
    case 0:
    {
      oled.Clear_Screen();
      i=0;
      while(i<4)
      {
        show_menu(i);
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
        if(i==1 && digitalRead(button)==LOW)
        {
          dis=2;
          break;
        }
        else if(i==3 && digitalRead(button)==LOW)
        {
          dis=1;
          break;
        }
        else if(i==0 && digitalRead(button)==LOW)
        {
          dis = 3;
          break;
        }
      }
      break;
    }
    case 1:
    {
      oled.Clear_Screen();
      show_stats();
      break;
    }
    case 2:
    {
      i=0;
      int control=0;
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
        if(i==0 && digitalRead(button)==LOW)
        {
          control = 1;
          break;
        }
        if(i==1 && digitalRead(button)==LOW)
        {
          control = 2;
          break;
        }
        if(i==2 && digitalRead(button)==LOW)
        {
          control = 3;
          break;
        }
        if(i==3 && digitalRead(button)==LOW)
        {
          dis=0;
          break;
        }
      }
      switch(control)
      {
        case 1:
          oled.Clear_Screen();
          while(digitalRead(button)!=LOW)
          {
            manual_control();
          }
          dis=2;
          break;
        case 2:
          oled.Clear_Screen();
          while(digitalRead(button)!=LOW)
          {
            voice_control();
          }
          dis=2;
          break;
        case 3:
          oled.Clear_Screen();
          while(digitalRead(button)!=LOW)
          {
            brainwave_control();
          }
          dis=2;
          break;
      }
      break;
    }
    case 3:
    {
      oled.Clear_Screen();
      Lock();
      break;
    }
  }
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
        dis=0;
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


void Lock()
{
  oled.Set_Color(WHITE);
  oled.print_String(32, 30, (const uint8_t*)"Enter the", FONT_8X16);
  oled.print_String(35, 45, (const uint8_t*)"joystick", FONT_8X16);
  oled.print_String(26, 60, (const uint8_t*)"combination:", FONT_8X16);
  while(true)
  {
    if((int)analogRead(A2) == 1023)
    {
      delay(500);
      if((int)analogRead(A2) == 0)
      {
        delay(500);
        if(digitalRead(button) == LOW)
        {
          dis=0;
          break;
        }
      }
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
    oled.Set_Color(RED);
  }
  oled.print_String(47, 38, (const uint8_t*)"Lock", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 1)
    oled.Set_Color(RED);
  oled.print_String(45, 53, (const uint8_t*)"Modes", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 2)
    oled.Set_Color(RED);
  oled.print_String(27, 68, (const uint8_t*)"Management", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 3)
    oled.Set_Color(RED);
  oled.print_String(47, 83, (const uint8_t*)"Stats", FONT_8X16);
  oled.Set_Color(WHITE);
  /*if(idx == 4)
  {
    oled.Set_Color(RED);
  }
  oled.print_String(47, 98, (const uint8_t*)"BACK", FONT_8X16);  
  oled.Set_Color(WHITE);*/
}


void modes(int idx)
{
  oled.Set_Color(BLUE);
  oled.print_String(47, 5, (const uint8_t*)"MODES", FONT_8X16);
  draw_line(0, 23, 1);
  oled.Set_Color(WHITE);
  if(idx == 0)
  {
    oled.Set_Color(RED);
  }
  oled.print_String(44, 38, (const uint8_t*)"Manual", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 1)
    oled.Set_Color(RED);
  oled.print_String(47, 53, (const uint8_t*)"Voice", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 2)
    oled.Set_Color(RED);
  oled.print_String(35, 68, (const uint8_t*)"Brainwave", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 3)
    oled.Set_Color(RED);
  oled.print_String(50, 83, (const uint8_t*)"BACK", FONT_8X16);  
  oled.Set_Color(WHITE);
}


void manual_control()
{
  oled.Set_Color(BLUE);
  oled.print_String(8, 5, (const uint8_t*)"MANUAL_CONTROL", FONT_8X16);
  draw_line(0, 23, 1);
  oled.Set_Color(WHITE);
  oled.print_String(33, 38, (const uint8_t*)"Press the", FONT_8X16);
  oled.Set_Color(WHITE);
  oled.print_String(36, 53, (const uint8_t*)"joystick", FONT_8X16);
  oled.Set_Color(WHITE);
  oled.print_String(33, 68, (const uint8_t*)"for exit!", FONT_8X16);
  oled.Set_Color(WHITE);
  
}


void voice_control()
{
  oled.Set_Color(BLUE);
  oled.print_String(11, 5, (const uint8_t*)"VOICE_CONTROL", FONT_8X16);
  draw_line(0, 23, 1);
  oled.Set_Color(WHITE);
  oled.print_String(33, 38, (const uint8_t*)"Press the", FONT_8X16);
  oled.Set_Color(WHITE);
  oled.print_String(36, 53, (const uint8_t*)"joystick", FONT_8X16);
  oled.Set_Color(WHITE);
  oled.print_String(33, 68, (const uint8_t*)"for exit!", FONT_8X16);
  oled.Set_Color(WHITE);
}


void brainwave_control()
{
  oled.Set_Color(BLUE);
  oled.print_String(11, 5, (const uint8_t*)"BRAIN_CONTROL", FONT_8X16);
  draw_line(0, 23, 1);
  oled.Set_Color(WHITE);
  oled.print_String(33, 38, (const uint8_t*)"Press the", FONT_8X16);
  oled.Set_Color(WHITE);
  oled.print_String(36, 53, (const uint8_t*)"joystick", FONT_8X16);
  oled.Set_Color(WHITE);
  oled.print_String(33, 68, (const uint8_t*)"for exit!", FONT_8X16);
  oled.Set_Color(WHITE);
}
