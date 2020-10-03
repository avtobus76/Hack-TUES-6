#include <stdio.h>
#include <stdlib.h>
#include "routes.h"
#include <Arduino.h>
struct route_t* init_route(unsigned int index)
{
	struct route_t* new_route = (struct route_t*)malloc(sizeof(struct route_t));

	new_route->start = NULL;
	new_route->end = NULL;
	new_route->size = 0;
	new_route->index = index;

	return new_route;
}

unsigned char invert_command(unsigned char command)
{
	if(command == 'F')
		return 'B';
	else if(command == 'B')
		return 'F';
	else if(command == 'L')
		return 'R';

	return 'L';
}

void erase_route_SRAM(struct route_t* route)
{
  
  while(route->size != 0)
  {
    delete_command(route, START);
    route->size --;
  }
    

  free(route);
}
void add_command(struct route_t* route, unsigned char command, unsigned long duration, unsigned int gear)
{
	if(route->size == 32)
		return;

	struct command_t* new_command;
	new_command = (struct command_t*) malloc(sizeof(*new_command));

	new_command->command = command;
	new_command->duration = duration;
	new_command->gear = gear;

	if(route->size == 0)
	{
		route->start = route->end = new_command;
		new_command->previous_command = new_command->next_command = NULL;
	}
	else
	{
		route->end->next_command = new_command;
		new_command->previous_command = route->end;
		new_command->next_command = NULL;
		route->end = new_command;
	}

	route->size += 1;
}

void delete_command(struct route_t* route, enum starting_point begin_from)
{
	struct command_t *deleted_command;
	switch(begin_from)
	{
		case START:
			route->start = route->start->next_command;
			deleted_command = route->start->previous_command;
			route->start->previous_command = NULL;
			free(deleted_command);
			break;

		case END:
			route->end = route->end->previous_command;
			deleted_command = route->end->next_command;
			route->end->next_command = NULL;
			free(deleted_command);
			break;
	}

	route->size -= 1;
}

void perform_route(struct route_t route, enum perform_direction direction, void (*perform_function)(unsigned char, unsigned long, unsigned int), unsigned char (*invert)(unsigned char))
{
	struct command_t* current_command;

	switch(direction)
	{
		case FORWARD:
			current_command = route.start;

			while(current_command != NULL)
			{
				perform_function(current_command->command, current_command->duration, current_command->gear);
				current_command = current_command->next_command;
			}
			break;

		case REVERSE:
			current_command = route.end;

			while(current_command != NULL)
			{
				perform_function(invert_command(current_command->command), current_command->duration, current_command->gear);
				current_command = current_command->previous_command;
			}
			break;
	}
}

void upload_route(struct route_t* route)
{
	struct command_t* current_command = route->start;
	unsigned int addr;
 
	if(route->index > 0 && route->index < 6)
  {
    erase_route_EEPROM(route->index);
    addr = (route->index - 1)*128;
  }
		  
	for(int curr = 0; curr < route->size; curr ++)
	{
		memory_write(addr, current_command->command);
		memory_write(addr+1, (current_command->duration)>>8);
		memory_write(addr+2, (current_command->duration)&0xFF);
		memory_write(addr+3, current_command->gear);    
		current_command = current_command->next_command;
		addr += 4;
	}
 
}

struct route_t* download_route(int idx, int n)
{
  struct route_t* new_route = init_route(idx);
  unsigned int addr = (idx-1)*128;

  for(int a = 0; a < n; ++a)
  {
    char com = memory_read(addr);
    long dur = (memory_read(addr+1)<<8)|(memory_read(addr+2)&0xFF);
    int gear =  memory_read(addr+3);
    add_command(new_route, com, dur, gear);
    addr += 4;
  }

	return new_route;
}

void erase_route_EEPROM(int idx)
{
  unsigned int addr = idx*128-1;
  while(addr != 0)
  {
    memory_write(addr-127, 0xFF);
    --addr;
  }
}
