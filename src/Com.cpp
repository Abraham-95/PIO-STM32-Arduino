#include "Com.h"

DS_Control_Data_Mode ds_control_data;
DS_Settings_Data_Package ds_settings_data;
Robot_Settings_Data_Package robot_settings_data;
Robot_Sensor_Data_Package robot_sensor_data;

unsigned long ds_last_received_time = 0;

byte receiveType = DS_CONTROL_DATA;
bool controllerConnected = false;

void setupCom() {
  SerialUSB.begin(115200);
  Serial3.begin(115200);

  pinMode(LED_GREEN, OUTPUT); digitalWrite(LED_GREEN, LOW);
  initializeControllerPayload();
}

void blinkLED() {
  digitalWrite(LED_GREEN, HIGH); delay(100); digitalWrite(LED_GREEN, LOW); delay(50);
}

void initializeControllerPayload() {
    // control package
    ds_control_data.type = DS_CONTROL_DATA;

    ds_control_data.axisX = 0; ds_control_data.axisY = 0;
    ds_control_data.axisRX = 0; ds_control_data.axisRY = 0;
    ds_control_data.gyro_X = 0; ds_control_data.gyro_Y = 0;
    ds_control_data.gyro_Z = 0;
    ds_control_data.buttons = UNPRESSED;
    ds_control_data.misc = UNPRESSED;
    ds_control_data.dpad = UNPRESSED;

    // settings package
    ds_settings_data.type = DS_SETTINGS_DATA;
}

bool receiveComData() {
  static enum {
    WAIT_HEADER1, WAIT_HEADER2, WAIT_TYPE, WAIT_LENGTH, WAIT_DATA, WAIT_CHECKSUM
  } state = WAIT_HEADER1;

  static uint8_t type, length, data[16], index, checksum;
  static DS_Control_Data_Mode lastData;

  while (Serial3.available()) {
    uint8_t byteIn = Serial3.read();
    switch (state) {
      case WAIT_HEADER1: {
        checksum = 0;
        if (byteIn == HEADER1) {state = WAIT_HEADER2;} break;}
      case WAIT_HEADER2: {
        if (byteIn == HEADER2) {state = WAIT_TYPE;} else {state = WAIT_HEADER1;} break;}
      case WAIT_TYPE: {
        type = byteIn; checksum = byteIn; state = WAIT_LENGTH; break;}
      case WAIT_LENGTH: {
        length = byteIn; checksum ^= byteIn;
        if (length > sizeof(data)) {state = WAIT_HEADER1; break;}
        index = 0; state = WAIT_DATA;  break;}
      case WAIT_DATA: {
        data[index++] = byteIn; checksum ^= byteIn;
        if (index >= length) {state = WAIT_CHECKSUM;} break;}
      case WAIT_CHECKSUM: {
        if (checksum == byteIn) {
          if (type == TYPE_CONTROL && length == 7) {
            ds_control_data.buttons   = data[0];
            ds_control_data.misc      = data[1];
            ds_control_data.dpad      = data[2];
            ds_control_data.axisX     = (int8_t)data[3];
            ds_control_data.axisY     = (int8_t)data[4];
            ds_control_data.axisRX    = (int8_t)data[5];
            ds_control_data.axisRY    = (int8_t)data[6];

            ds_last_received_time = millis();
          }
          state = WAIT_HEADER1; break;
        }
      }
    }
  }
  bool changed =
  (lastData.buttons != ds_control_data.buttons) ||
  (lastData.misc    != ds_control_data.misc) ||
  (lastData.dpad    != ds_control_data.dpad) ||
  (lastData.axisX   != ds_control_data.axisX) ||
  (lastData.axisY   != ds_control_data.axisY) ||
  (lastData.axisRX  != ds_control_data.axisRX) ||
  (lastData.axisRY  != ds_control_data.axisRY);

  if (millis() - ds_last_received_time > INTERVAL_MS_SIGNAL_LOST) {
    ds_control_data.axisX = 0; ds_control_data.axisY = 0;
    ds_control_data.axisRX = 0; ds_control_data.axisRY = 0;
    ds_control_data.buttons = 0;
    ds_control_data.dpad = 0;
    ds_control_data.misc = 0;
    return false;
  }
  return true;
}

ButtonEvent readButtonEvent() {
  static uint8_t prevButtons = 0; static uint8_t prevDpad    = 0;
  ButtonEvent event = BUTTON_NONE;
  uint8_t b = ds_control_data.buttons; uint8_t d = ds_control_data.dpad;

  if ((b & 0x01) && !(prevButtons & 0x01)) event = BUTTON_X;
  else if ((b & 0x04) && !(prevButtons & 0x04)) event = BUTTON_SQUARE;
  else if ((b & 0x08) && !(prevButtons & 0x08)) event = BUTTON_TRIANGLE;
  else if ((b & 0x02) && !(prevButtons & 0x02)) event = BUTTON_CIRCLE;
  else if ((d & 0x01) && !(prevDpad & 0x01)) event = BUTTON_DPAD_UP;
  else if ((d & 0x02) && !(prevDpad & 0x02)) event = BUTTON_DPAD_DOWN;
  else if ((d & 0x08) && !(prevDpad & 0x08)) event = BUTTON_DPAD_LEFT;
  else if ((d & 0x04) && !(prevDpad & 0x04)) event = BUTTON_DPAD_RIGHT;

  prevButtons = b; prevDpad = d;
  //if(event != BUTTON_NONE) blinkLED();
  return event;
}



