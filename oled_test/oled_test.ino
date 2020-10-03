#include <EEPROM.h>
#include "OLED_Driver.h"
#include "OLED_GFX.h"
#include <SPI.h>
//#include <Wire.h>

//#define SLAVE_ADDR 0x15
//#define JOYSTICK_READY 5
//#define SLAVE_READY 6

#define button 3
OLED_GFX oled = OLED_GFX();

//#define RECORDED 128

enum cmds{_STOP, _FORWARD, _BACKWARD, _LEFT, _RIGHT};
enum modules{VOICE_RECOGNITION_MODULE = 1, BRAINWAVE_MODULE};

enum output_data_t_vars{is_recording, module_enable, rec_number};
enum input_data_t_vars{voice_command, brainwave_command};

char input_data_t[2] = {0, 0};
char output_data_t[3] = {0, 0, 0};

enum addrs{MANUAL_ADDR = 100, VOICE_ADDR, BRAIN_ADDR};

uint8_t* sp_1;
uint8_t* sp_2;

int count=0;
int i=0;
int oled_count=0;
int dis = 0;
int manual=0;
int voice=0;
int brain=0;
int light=0;

void show_menu(int, int);
void modes(int);
void Lock();
void save_routes();
void managment(int);
void save_voice_commands(int);
void show_stats();
//void _transcieve(uint8_t*, uint8_t*);

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

int flag=0;

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
  Serial.begin(57600);
  //pinMode(JOYSTICK_READY, OUTPUT);
  //pinMode(SLAVE_READY, INPUT);
  //digitalWrite(JOYSTICK_READY, LOW);
  
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
  switch(dis)
  {
    case 0:
    {
      show_stats();
      break;
    }
    case 1:
    {
      if(oled_count==0)
      {
        oled.Clear_Screen();
        oled_count=1;
        i=0;
      }

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
          
      if(i>4)i=0;
      if(i<0)i=4;
      
      show_menu(i);
      
      if(i==1 && digitalRead(button)==LOW)
      {
        oled_count=0;
        dis=2;
        break;
      }
      else if(i==4 && digitalRead(button)==LOW)
      {
        oled_count=0;
        dis=0;
        break;
      }
      else if(i==0 && digitalRead(button)==LOW)
      {
        oled_count=0;
        dis = 3;
        break;
      }
      else if(i==3 && digitalRead(button)==LOW)
      {
        oled_count=0;
        dis = 4;
        break;
      }
      else if(i==2 && digitalRead(button)==LOW)
      {
        oled_count=0;
        dis=5;
        break;
      }
      
      break;
    }
    case 2:
    {
      if(oled_count==0)
      {
        oled.Clear_Screen();
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

      if(i>4)i=0;
      if(i<0)i=4;
      
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
        oled_count=0;
        dis=1;
        break;
      }
      if(dis==2)
      {
        oled_count=1;
      }
      break;
    }
    case 3:
    {
      if(oled_count==0)
      {
        oled.Clear_Screen();
      }
      Lock();
      if(dis==3)
      {
        oled_count=1;
      }
      break;
    }
    case 4:
    {
      if(oled_count==0)
      {
        oled.Clear_Screen();
      }
      save_routes();
      oled_count=0;
      dis=1;
      if(dis==4)
      {
        oled_count=1;
      }
      break;
    }
    case 5:
    {
      if(oled_count==0)
      {
        oled.Clear_Screen();
        count=0;
      }
      managment(count);
      if(((int)analogRead(A1) > 256) && ((int)analogRead(A1) < 768) && (flag!=0))
      {
        flag=0;
      }
      if((int)analogRead(A1) < 256 && (flag!=-1))
      {
        count++;
        //delay(100);
        flag=-1;
      }
      if((int)analogRead(A1) > 768 && (flag!=1))
      {
        count--;
        //delay(100);
        flag=1;
      }
      if(count>3)count=0;
      if(count<0)count=3;
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
        oled_count=0;
        dis=1;
        break;
      }
      if(dis==5)
      {
        oled_count=1;
      }
      break;
    }
    case 6:
    {
      if(oled_count==0)
      {
        oled.Clear_Screen();
        count=0;
      }
      save_voice_commands(count);
      if(((int)analogRead(A1) > 256) && ((int)analogRead(A1) < 768) && (flag!=0))
      {
        flag=0;
      }
      if((int)analogRead(A1) < 256 && (flag!=-1))
      {
        count++;
        //delay(100);
        flag=-1;
      }
      if((int)analogRead(A1) > 768 && (flag!=1))
      {
        count--;
        //delay(100);
        flag=1;
      }
      if(count>6)count=0;
      if(count<0)count=6;
      if(count==0 && digitalRead(button)==LOW)
      {
        output_data_t[rec_number]=4;
      }
      if(count==1 && digitalRead(button)==LOW)
      {
        output_data_t[rec_number]=3;
      }
      if(count==2 && digitalRead(button)==LOW)
      {
        output_data_t[rec_number]=2;
      }
      if(count==3 && digitalRead(button)==LOW)
      {
        output_data_t[rec_number]=1;
      }
      if(count==4 && digitalRead(button)==LOW)
      {
        output_data_t[rec_number]=0;
      }
      if(count==5 && digitalRead(button)==LOW)
      {
        oled_count=0;
        dis=5;
        break;
      }
      if(dis==6)
      {
        oled_count=1;
      }
      break;
    }
  }
  /*int counter=0;
  while(counter<3)
  {
    Serial.print((int)output_data_t[counter]);
    counter++;
  }
  Serial.println();
   
  counter=0;
  while(counter<2)
  {
    Serial.print((int)input_data_t[counter]);
    counter++;
  }
  Serial.println();
  _transcieve(input_data_t, output_data_t);*/
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
    if((int)analogRead(A2) > 768)
    {
      Serial.println((int)analogRead(A1));
      delay(600);
      if((int)analogRead(A1) < 256)
      {
        Serial.println((int)analogRead(A2));
        oled_count=0;
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

/*void _transcieve(uint8_t* input, uint8_t* output)
{
  digitalWrite(JOYSTICK_READY, HIGH);
  while(digitalRead(SLAVE_READY)!=HIGH);
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(output, 3);
  Wire.endTransmission(1);
  Wire.requestFrom(SLAVE_ADDR, 2);
  int idx = 0;
  while(Wire.available())
  {
    input[idx] = Wire.read();
    idx++; 
  }
  digitalWrite(JOYSTICK_READY, LOW);
}*/
