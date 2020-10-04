#include "Common.h"

void setup() {
  init_alarm();
  oled_setup();
  _initAirHumSensor();
  _initButtons();
  init_motors_temp();
  init_communication();
}

void loop() {
  

  mc_data_in[VR_COMMAND] = voice_commands;
  mc_data_in[BRAKES] = 0;

  if (mc_data_out[ALARM])
    engage_alarm();
  else
    disengage_alarm();

  if (mc_data_out[RECORDING])
  {
    save_routes();
  }

  update_oled();
  _getHornButtonState();
  _getParkButtonState();
  _getGearUpButtonState();
  _getGearDownButtonState();

  if (gear_up_button_state == 1 && do_ones_up == 0)
  {
    gearUp();
    do_ones_up = 1;
  }

  if (gear_down_button_state == 1 && do_ones_down == 0)
  {
    gearDown();
    do_ones_down = 1;
  }

  mc_data_in[GEAR] = curr_gear;
  _ShowGear(gear + curr_gear);

  if (curr_gear == _NEUTRAL && park_button_state == 1)
  {
    curr_gear = _PARK;
    mc_data_in[GEAR] = _PARK;
    _ShowGear(gear + _PARK);
    mc_data_in[BRAKES] = 1;
  }
  
  transcieve();
  vr3_data_in[IS_RECORDING] = 0;
}
