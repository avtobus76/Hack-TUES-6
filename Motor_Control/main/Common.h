#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <PWM.h>
#include <ShiftRegister74HC595.h>
#include <inttypes.h>
#include <SPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <Wire.h>
#include <EEPROM.h>

#define mem_select 8
#define MC_ADDR 0x10

struct settings
{
  uint8_t _gear;
  uint8_t _direction;  
}settings;

int data_in[6];
int data_out[2];

#define MAX_ADDR 65535

enum starting_point{START, END};
enum perform_direction{FORWARD, REVERSE};
enum _gears{FIRST, SECOND, THIRD};

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

enum keys{BT_KEY=1, ONBOARD_KEY};
enum directions{_STOP, _FORWARD, _BACKWARD, _LEFT, _RIGHT};
enum gears{_PARK, _NEUTRAL, _FIRST, _SECOND, _THIRD};
enum control_bits{PWM_SELECT1, PWM_SELECT2, SD1, SD2, LIN1, LIN2, LIN3, LIN4};
enum data_in_vars{MODE, GEAR, BRAKES, IN_MENU, VR_COMMAND, SAVE_ROUTE};
enum data_out_vars{ALARM, IS_RECORDING};

char bt_command;
char bt_key_stat = '0';
int kolichka_on = 0;
int recording = 0;
int route_index;
long dur;
char com;
int saved = 0;

const int power_supply_pin = A2;
const int brakes_pin = 5;
const int onboard_key_pin = 7;
const int CS = A3;
const int RESP = 3;
const int joystick_x = A1;
const int joystick_y = A0;
const int left_motor = 9;
const int right_motor = 10;
const int mc_register_data_pin= 2;
const int mc_register_clk_pin = 4;
const int mc_register_latch_pin = 6;

ShiftRegister74HC595<1> control_reg(mc_register_data_pin, mc_register_clk_pin, mc_register_latch_pin);

struct route_t* route;

void init_ps(); //gotova
void init_bluetooth(); //gotova
void init_alarm(); //gotova
void init_brakes(); //gotova
void init_joystick(); //gotova
int check_key_status(int); //gotova
void engage_brakes(); //gotova
void disengage_brakes(); //gotova
char read_bt_command(); //gotova
char get_joystick_position(); //gotova
void _InitMotors(); //gotova
void _move(struct settings);
void _UpdateControlReg(uint8_t*); 
void _SetSpeed(int, uint8_t);
struct route_t* init_route(unsigned int);
unsigned char invert_command(unsigned char);
void add_command(struct route_t*, unsigned char, unsigned long, unsigned int);
void delete_command(struct route_t*, enum starting_point);
void perform_route(struct route_t, enum perform_direction, void (*)(unsigned char, unsigned long, unsigned int), unsigned char (*)(unsigned char));
void upload_route(struct route_t*);
struct route_t* download_route();
void erase_route(struct route_t*);
void erase_pageEEPROM(int);
void engage_horn();
void disengage_horn();
void init_communication();
void receive(int);
void transmit();
void run_motors(unsigned char, unsigned long, unsigned int);
char dir2com(int);
int com2dir(char);

#endif
