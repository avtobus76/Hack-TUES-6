#include "OLED_Driver.h"
#include "OLED_GFX.h"
#include <SPI.h>

OLED_GFX oled = OLED_GFX();

uint8_t* sp_1;
uint8_t* sp_2;

int dis = 0;

void show_stats();
void show_menu(int, int);
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

  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();

  oled.Device_Init();
  oled.Clear_Screen();
  oled.Set_Color(RED);
  oled.print_String(29, 54, (const uint8_t*)"AVTOBUS 76", FONT_8X16);
  delay(2500);
  oled.print_String(12, 54, (const uint8_t*)"Smart Kolichka", FONT_8X16);
  delay(5000);
  oled.Clear_Screen();
}

void loop() {
  switch(dis)
  {
    case 0:
      show_stats();
      break;

    case 1:
      oled.Clear_Screen();
      for (int i = 0; i < 5; i++)
        show_menu(i, i==5?1:0);
      break;
  }
  dis ++;
  if (dis == 2)
    dis = 0;
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

void show_menu(int idx, int selected)
{
  oled.Set_Color(BLUE);
  oled.print_String(47, 5, (const uint8_t*)"MENU", FONT_8X16);
  draw_line(0, 23, 1);
  oled.Set_Color(WHITE);
  if(idx == 0)
  {
    oled.Set_Color(RED);
    if(selected == 1)
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
  oled.print_String(39, 83, (const uint8_t*)"Route66", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 4)
  {
    oled.Set_Color(RED);
    if(selected == 1)
      dis = 0 
  }
  oled.print_String(47, 98, (const uint8_t*)"BACK", FONT_8X16);  
  oled.Set_Color(WHITE);
  delay(3000);
}
