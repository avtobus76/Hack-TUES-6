#include"Common.h"
#include <EEPROM.h>
#include "OLED_Driver.h"
#include "OLED_GFX.h"
#include <SPI.h>



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



void oled_setup()
{
  pinMode(record_led, OUTPUT);
  digitalWrite(record_led, LOW);
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
  //brain = EEPROM.read(BRAIN_ADDR);

  oled.Device_Init();
  oled.Clear_Screen();
  oled.Set_Color(RED);
  oled.print_String(29, 54, (const uint8_t*)"AVTOBUS 76", FONT_8X16);
  delay(2500);
  oled.print_String(12, 54, (const uint8_t*)"Smart Kolichka", FONT_8X16);
  delay(5000);
  oled.Clear_Screen();
}



void update_oled()
{
  switch(cases)
  {
    case 0://cases==0 funkciq "show_stats()" - upravlenie na kolichkata
    {
      if(oled_count==0)
      {
        oled.Clear_Screen();
        oled_count=1;
        i=0;
      }
      show_stats();
      if(digitalRead(button)==LOW)
        oled_count = 0;
      break;
    }

    case 1://cases==1 funkciq "show_menu()" - pokazva menu-to
    {
      if(oled_count==0)
      {
        oled.Clear_Screen();
        oled_count=1;
        i=0;
      }

      show_menu(i);
      
      if(((int)analogRead(A1) > 256) && ((int)analogRead(A1) < 768) && (flag!=0))
      {
        flag=0; //tozi "flag" pravi taka che da moje da se mesti samo vednuj nagore i nadolu, a ne pri zadurjane da se mesti nqkolko puti
      }
        
      if((int)analogRead(A1) < 256 && (flag!=-1))
      {
        i++;
        flag=-1;
      }
      if((int)analogRead(A1) > 768 && (flag!=1))
      {
        i--;
        flag=1;
      }
          
      if(i>3)i=0;
      if(i<0)i=3;
      
      if(i==1 && digitalRead(button)==LOW)
      {
        oled_count=0;
        cases=2;
        break;
      }
      else if(i==3 && digitalRead(button)==LOW)
      {
        oled_count=0;
        cases=0;
        break;
      }
      else if(i==0 && digitalRead(button)==LOW)
      {
        oled_count=0;
        cases = 3;
        break;
      }
      else if(i==2 && digitalRead(button)==LOW)
      {
        vr3_data_in[IS_RECORDING]=1;
        Serial.println(vr3_data_in[IS_RECORDING]);
      }
      break;
    }

    case 2://cases==2 funkciq "modes()" - izbirane na nachin za karane
    {
      if(oled_count==0)
      {
        oled.Clear_Screen();
        oled_count=1;
        i=0;
      }
      modes(i);
     
      if(((int)analogRead(A1) > 256) && ((int)analogRead(A1) < 768) && (flag!=0))
      {
        flag=0;
      }
      if((int)analogRead(A1) < 256 && (flag!=-1))
      {
        i++;
        //delay(100);
        flag=-1;
      }
      if((int)analogRead(A1) > 768 && (flag!=1))
      {
        i--;
        //delay(100);
        flag=1;
      }

      if(i>3)i=0;
      if(i<0)i=3;
      
      if(i==0 &&digitalRead(button)==LOW)
      {
        if(manual==1)
        {
          manual=0;
        }
        else
        {
          manual=1; brain=0; voice=0;
        }
      }
      if(i==1 && digitalRead(button)==LOW)
      {
        if(voice==1)
        {
          voice=0;
        }
        else
        {
          voice=1; brain=0; manual=0;
        }
      }
      if(i==2 && digitalRead(button)==LOW)
      {
        if(brain==1)
        {
          brain=0;
        }
        else
        {
          brain=1; voice=0; manual=0;
        }
      }
      if(i==3 && digitalRead(button)==LOW)
      {
        oled_count=0;
        cases=1;
        break;
      }
      break;
    }

    case 3://cases==3 funkciq "Lock()" - zakluchva kolichkata
    {
      if(oled_count==0)
      {
        oled.Clear_Screen();
        oled_count=1;
      }
      Lock();
      break;
    }
  }
}


void show_stats()
{
  mc_data_in[IN_MENU]=1;
  oled.Set_Color(BLUE);
  oled.print_String(47, 5, (const uint8_t*)"STATS", FONT_8X16);
  draw_line(0, 23, 1);

  oled.Set_Color(WHITE);
  oled.print_String(0, 38, (const uint8_t*)"Battery: ", FONT_8X16);
  oled.print_String(0, 53, (const uint8_t*)"LM Temp: ", FONT_8X16);
  oled.print_String(0, 68, (const uint8_t*)"RM Temp: ", FONT_8X16);
  oled.print_String(0, 83, (const uint8_t*)"Humidity: ", FONT_8X16);
  oled.print_String(0, 98, (const uint8_t*)"Air Temp: ", FONT_8X16);

  char battery_level[3];
  int bat_lvl = get_battery_level();
  if(numDigits(prev_bat_lvl) != numDigits(bat_lvl))
  {
    oled.Clear_Screen();
    if(numDigits(bat_lvl)==3)
    {
      itoa(bat_lvl, battery_level, 10);
      oled.print_String(73, 38, (const uint8_t*)battery_level, FONT_8X16);
      oled.print_String(100, 38, "%", FONT_8X16);
    }
    else if(numDigits(bat_lvl)==2)
    {
      itoa(bat_lvl, battery_level, 10);
      oled.print_String(73, 38, (const uint8_t*)battery_level, FONT_8X16);
      oled.print_String(90, 38, "%", FONT_8X16);
    }
    else
    {
      itoa(bat_lvl, battery_level, 10);
      oled.print_String(73, 38, (const uint8_t*)battery_level, FONT_8X16);
      oled.print_String(83, 38, "%", FONT_8X16);
    }
  }
  else
  {
    if(numDigits(bat_lvl)==3)
    {
      itoa(bat_lvl, battery_level, 10);
      oled.print_String(73, 38, (const uint8_t*)battery_level, FONT_8X16);
      oled.print_String(100, 38, "%", FONT_8X16);
    }
    else if(numDigits(bat_lvl)==2)
    {
      itoa(bat_lvl, battery_level, 10);
      oled.print_String(73, 38, (const uint8_t*)battery_level, FONT_8X16);
      oled.print_String(90, 38, "%", FONT_8X16);
    }
    else
    {
      itoa(bat_lvl, battery_level, 10);
      oled.print_String(73, 38, (const uint8_t*)battery_level, FONT_8X16);
      oled.print_String(83, 38, "%", FONT_8X16);
    }
  }
  prev_bat_lvl = bat_lvl;


  
  char left_motor_temp[2];
  int lmt = get_motor_temp(0);
  
  itoa(lmt, left_motor_temp, 10);
  oled.print_String(73, 53, (const uint8_t*)left_motor_temp, FONT_8X16);
  oled.print_String(90, 53, "C", FONT_8X16);

  
  char right_motor_temp[2];
  int rmt = get_motor_temp(1);
  
  itoa(rmt, right_motor_temp, 10);
  oled.print_String(73, 68, (const uint8_t*)right_motor_temp, FONT_8X16);
  oled.print_String(90, 68, "C", FONT_8X16);

  

  char _hum[3];
  char _temp[2];
  int hum, temp;
  _getAirTempHum(&temp, &hum);
    
  itoa(hum, _hum, 10);
  itoa(temp, _temp, 10);
  oled.print_String(80, 83, (const uint8_t*)_hum, FONT_8X16);
  oled.print_String(95, 83, "%", FONT_8X16);
  oled.print_String(80, 98, (const uint8_t*)_temp, FONT_8X16);
  oled.print_String(95, 98, "C", FONT_8X16);

  

  if(manual==1)
  {
    vr3_data_in[MODULE_ENABLE]=1;
    mc_data_in[MODE]=1;
  }
  else if(voice==1)
  {
    vr3_data_in[MODULE_ENABLE]=2;
    mc_data_in[MODE]=2;
  }
  else
  {
    vr3_data_in[MODULE_ENABLE]=0;   
    mc_data_in[MODE]=0; 
  }
  if(digitalRead(button) == LOW)
  {
    EEPROM.write(VOICE_ADDR, voice);
    EEPROM.write(MANUAL_ADDR, manual);
    mc_data_in[IN_MENU]=0;
    
    cases=1;
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
  oled.print_String(27, 68, (const uint8_t*)"Voice_rec", FONT_8X16);
  oled.Set_Color(WHITE);
  if(idx == 3)
    oled.Set_Color(GREEN);
  oled.print_String(45, 83, (const uint8_t*)"Stats", FONT_8X16);
  oled.Set_Color(WHITE);
}








void modes(int idx)
{
  oled.Set_Color(BLUE);
  oled.print_String(47, 5, (const uint8_t*)"MODES", FONT_8X16);
  draw_line(0, 23, 1);
  if(manual==1 && brain==0 && voice==0)
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
  if(voice==1 && manual==0 && brain==0)
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
  if(brain==1 && manual==0 && voice==0)
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
    
    if((int)analogRead(A2) > 768)
    {
      delay(500);
      if((int)analogRead(A1) < 256)
      {
        oled_count=0;
        cases=1;
        break;
      }
    }
  }
}




void save_routes()
{
  if(once_clear_oled==0)
  {
    oled.Clear_Screen();
    once_clear_oled=1;
  }
  oled.Set_Color(WHITE);
  oled.print_String(60, 9, (const uint8_t*)"Start recording:", FONT_8X16);
  
  mc_data_in[SAVE_ROUTE]=1;
  if(mc_data_out[RECORDING]==0)
  {
    mc_data_in[SAVE_ROUTE]=0;
    oled.Clear_Screen();
    once_clear_oled=0;
    cases=1;
  }
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
  oled.print_String(29, 40, (const uint8_t*)"Headlights", FONT_8X16);
  oled.Set_Color(BLUE);
  if(idx==2)
    oled.Set_Color(GREEN);
  oled.print_String(47, 55, (const uint8_t*)"Back", FONT_8X16);
  oled.Set_Color(WHITE);
}
