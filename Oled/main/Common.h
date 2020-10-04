#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <OneWire.h> 
#include <DallasTemperature.h>
#include <DHT.h>
#include <ShiftRegister74HC595.h>
#include <EEPROM.h>
#include "OLED_Driver.h"
#include "OLED_GFX.h"
#include <SPI.h>
#include <Wire.h>

#define ONE_WIRE_BUS 12
#define button 22
#define VR3_ADDR 0x15
#define MC_ADDR 0x10

OLED_GFX oled = OLED_GFX();

int curr_gear = 0;
int do_ones_up = 0;
int do_ones_down = 0;

enum addrs{MANUAL_ADDR = 100, VOICE_ADDR, BRAIN_ADDR};
enum gears{_PARK, _NEUTRAL, _FIRST, _SECOND, _THIRD};
enum vr3_input_vars{IS_RECORDING, MODULE_ENABLE};
enum slaves{VR3, MC};
enum mc_input_vars{MODE, GEAR, BRAKES, IN_MENU, VR_COMMAND, SAVE_ROUTE};
enum mc_output_vars{ALARM, RECORDING};

uint8_t gear[5] = {115, 63, 6, 91, 79};
uint8_t mc_data_in[6] = {0, 0, 0, 0, 0, 0};
uint8_t mc_data_out[2] = {0, 0};
uint8_t vr3_data_in[2] = {0, 0};
uint8_t voice_commands = 0;

uint8_t* sp_1;
uint8_t* sp_2;
int horn_button_state = 0;
int park_button_state = 0;
int gear_up_button_state = 0;
int gear_down_button_state = 0;

int go_one=0;
int once_clear_oled = 0;
int prev_bat_lvl = 0;
int flag = 0;
int i = 0;
int oled_count = 0;
int cases = 0;
int manual = 0;
int voice = 0;
int brain = 0;

const int gearUp_button = 20;
const int gearDown_button = 21;
const int mc_register_data_pin = 8;
const int mc_register_clk_pin = 10;
const int mc_register_latch_pin = 9;
const int air_sensor_pin = 5;
const int horn_button = 19;
const int park_button = 18;
const int battery_level_pin = A5;
const int VR3_CS = 30;
const int VR3_RESP = 31;
const int MC_CS = 32;
const int MC_RESP = 33;
const int alarm_pin = 23;
const int record_led = 6;

DHT dht(air_sensor_pin, DHT22);

ShiftRegister74HC595<1> indicator(mc_register_data_pin, mc_register_clk_pin, mc_register_latch_pin);
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

void _ShowGear(uint8_t*);
void show_menu(int);
void draw_line(int, int, int);
void modes(int);
void Lock();
void save_routes();
void managment(int);
void save_voice_commands(int);
void show_stats();
void oled_setup();
void _initAirHumSensor();
void _getAirTempHum(int*, int*);
void _initButtons();
void _getHornButtonState();
void _getParkButtonState();
void init_motors_temp();
int get_motor_temp(int);
void init_battery();
int get_battery_level();
int numDigits(int);
void init_communication();
void transcieve();
void _getGearUpButtonState();
void _getGearDownButtonState();
void gearUp();
void gearDown();

int numDigits(int n)
{
  int num_digits = 0;
  
  while(n != 0)
  {
    ++num_digits;
    n /= 10;
  }
  
  return num_digits;
}

#endif
