#pragma once
#include <Arduino.h>

#define HEADER1 0xAA
#define HEADER2 0xBB
#define TYPE_CONTROL 0xCC

#define UNPRESSED 0x0
#define PRESSED 0x1

#define INTERVAL_MS_SIGNAL_LOST 1000
#define LED_PULSE_DURATION_MS 1000

enum EventCode {
  EVENT_CONNECTED    = 1,
  EVENT_DISCONNECTED = 2
};

enum PackageType {
  DS_CONTROL_DATA = 251,
  DS_SETTINGS_DATA = 252,
  ROBOT_SETTINGS_DATA = 253,
  ROBOT_SENSOR_DATA = 254
};

enum ButtonEvent {
  BUTTON_NONE, BUTTON_X, BUTTON_SQUARE, BUTTON_TRIANGLE, BUTTON_CIRCLE,
  BUTTON_DPAD_UP, BUTTON_DPAD_DOWN, BUTTON_DPAD_LEFT, BUTTON_DPAD_RIGHT
};

struct DS_Control_Data_Mode {
  uint8_t type;
  int8_t axisX; int8_t axisY; int8_t axisRX; int8_t axisRY;
  int8_t gyro_X; int8_t gyro_Y; int8_t gyro_Z;
  uint8_t buttons; uint8_t misc; uint8_t dpad;
};

struct DS_Settings_Data_Package {
  uint8_t type;
};

struct Robot_Settings_Data_Package {
  uint8_t type;
};

struct Robot_Sensor_Data_Package {
  uint8_t type;
};

extern DS_Control_Data_Mode ds_control_data;
extern DS_Settings_Data_Package ds_settings_data;
extern Robot_Settings_Data_Package robot_settings_data;
extern Robot_Sensor_Data_Package robot_sensor_data;

extern uint8_t receiveType;
extern bool controllerConnected;
extern ButtonEvent readButtonEvent();

void setupCom();
void updateLED();
bool receiveComData();
void initializeControllerPayload();



