#ifndef MEMORY_H
#define MEMORY_H

#include <inttypes.h>
#include <SPI.h>

#define mem_select 10

void memory_init()
{
	pinMode(mem_select, OUTPUT);
  SPI.begin();
}

uint8_t read_status()
{
  uint8_t stat = 0;

  digitalWrite(mem_select, LOW);
  SPI.transfer(0x05);
  stat = SPI.transfer(0);
  digitalWrite(mem_select, HIGH);

  return stat;
}

int is_wren()
{
  uint8_t stat = read_status();

  if(((stat >> 1) & 0x01)) 
    return 1;

  return 0;
}

int memory_write(uint16_t address, uint8_t data)
{
	  digitalWrite(mem_select, LOW);
	  SPI.transfer(0x06);
	  digitalWrite(mem_select, HIGH);

	  if(!is_wren()) return -1;

	  digitalWrite(mem_select, LOW);
	  SPI.transfer(0x02);
	  SPI.transfer(address>>8);
	  SPI.transfer(address);
	  SPI.transfer(data);
	  digitalWrite(mem_select, HIGH);

	  while((read_status()&0x01));

	  return !is_wren();
}

uint8_t memory_read(uint16_t addr)
{
  uint8_t res = 0;

  digitalWrite(mem_select, LOW);
  SPI.transfer(0x03);
  SPI.transfer(addr>>8);
  SPI.transfer(addr);
  res = SPI.transfer(0);
  digitalWrite(mem_select, HIGH);

  return res;
}

#endif
