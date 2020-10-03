#ifndef COMMON
#define COMMON

#include <Wire.h>
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

#define RECORDED 128
#define CMD_NUM 5
#define VR3_TGAM_ADDR 0x10

#define CS 6
#define RESPONSE 7

VR myVR(2,3);

enum cmds{_STOP, _FORWARD, _BACKWARD, _LEFT, _RIGHT};
enum modules{NO_MODULES=0, VOICE_RECOGNITION_MODULE=2};

enum input_data_t_vars{is_recording, module_enable};
enum output_data_t_vars{voice_command};

uint8_t input_data_t[2] = {0, NO_MODULES};
uint8_t output_data_t = 0;

uint8_t stat[50];

void _receive(int);
void _transmit();
void transcieve();
void _initCommunication();
void _initVR3();
void _clr();
void _record();

#endif
