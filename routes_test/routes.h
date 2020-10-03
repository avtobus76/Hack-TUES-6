#ifndef ROUTES_H
#define ROUTES_H

enum starting_point{START, END};
enum perform_direction{FORWARD, REVERSE};
enum gears{FIRST, SECOND, THIRD};

struct command_t
{	
	unsigned char command;
	unsigned long duration;
	unsigned int gear;
	struct command_t* previous_command;
	struct command_t* next_command;
};

struct route_t
{
	struct command_t* start;
	struct command_t* end;
	unsigned int size;
	unsigned int index;
};

struct route_t* init_route(unsigned int);
unsigned char invert_command(unsigned char);
void add_command(struct route_t*, unsigned char, unsigned long, unsigned int);
void delete_command(struct route_t*, enum starting_point);
void perform_route(struct route_t, enum perform_direction, void (*)(unsigned char, unsigned long, unsigned int), unsigned char (*)(unsigned char));
void upload_route(struct route_t*);
struct route_t* download_route(int, int);
void erase_route_SRAM(struct route_t*);
void erase_route_EEPROM(int);
void _erase(unsigned int);

#endif
