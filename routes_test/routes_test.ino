#include <EEPROM.h>
#include "memory.h"
#include "routes.h"
#include "routes.c"


void print_f(unsigned char com, unsigned long dur, unsigned int gear)
{
  Serial.println((char)com);
  Serial.println((long)dur);
  Serial.println((int)gear);
}

void setup()
{
  Serial.begin(9600);
  int index = 1, _size = 10;
  memory_init();
  struct route_t* route1 = init_route(index);
  for(int a = 0; a < _size; ++a)
  {
    add_command(route1, 'L', (a+1)*1000, SECOND);
  }
  
  Serial.println("Before uploading:");
  perform_route(*route1, FORWARD, print_f, NULL);
  upload_route(route1);
  EEPROM.write(route1->index, route1->size);
  erase_route_SRAM(route1);
  Serial.println();
  Serial.println();
  delay(3000);
  int size = EEPROM.read(index);
  route1 = download_route(index, size);
  Serial.println("After downloading:");
  perform_route(*route1, FORWARD, print_f, NULL);
  /*while(size_ != 0)
  {
    char com = memory_read(addr);
    long dur = (memory_read(addr+1)<<8)|(memory_read(addr+2)&0xFF);
    int gear =  memory_read(addr+3);
    Serial.println(com);
    Serial.println(dur);
    Serial.println(gear);
    add_command(new_route, com, dur, gear);
    addr += 4;
    size_ --;
  }*/
  
  //return new_route;
  //Serial.print("Route Index: ");
  /*Serial.println(index);
  Serial.print("Route Size: ");
  Serial.println(route->size);
  
  struct command_t* current_command = route->start;
  for(int i = 0; i < route->size; i ++)
  {
    Serial.print("Command ");
    Serial.println(i+1);
    Serial.println(current_command->command);
    Serial.println(current_command->duration);
    Serial.println(current_command->gear);
    Serial.println();
    current_command = current_command->next_command;
  } 
  //for(int i = 0; i < 10; i++)
  //{
    //Serial.println(memory_read(0));
    //Serial.println(memory_read(2));
  //}*/
}

void loop()
{
  
}
