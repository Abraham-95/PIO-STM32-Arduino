#include <Arduino.h>
#include "Com.h"
#include "Config.h"
#include "Mode.h"
#include "Utility.h"

StandbyMode *standbyMode = new StandbyMode();
ServoMode *servoMode = new ServoMode();
CarMode *carMode = new CarMode();
SpeedMode *speedMode = new SpeedMode();

Mode *currentMode = nullptr;
Mode *previousMode = nullptr;

enum ControlMode {MANUAL, AUTO};
ControlMode controlMode = MANUAL;

void setup() {
  unsigned long startTime = millis();
  while (!SerialUSB && millis() - startTime < 1000) {}

  setupCom();

  currentMode = standbyMode;
  previousMode = nullptr;
  currentMode->init();
  changeMode(carMode);
}

void loop() {
  static unsigned long previousLoopTime = 0;
  unsigned long currentLoopTime = micros();
  if (currentLoopTime - previousLoopTime < LOOP_PERIOD) return;
  previousLoopTime = currentLoopTime;

  updateLED();

  bool connected = receiveComData();
  if (!connected && currentMode != standbyMode) {changeMode(standbyMode);}

  if (receiveType == DS_CONTROL_DATA) {
    ButtonEvent event = readButtonEvent();
    switch (event) {
      case BUTTON_X: changeMode(carMode); break;
      case BUTTON_SQUARE: changeMode(standbyMode); break;
      case BUTTON_TRIANGLE: changeMode(servoMode); break;
      case BUTTON_CIRCLE: changeMode(speedMode); break;
      case BUTTON_DPAD_UP: break;
      case BUTTON_DPAD_DOWN: break;
      case BUTTON_DPAD_RIGHT: break;
      case BUTTON_DPAD_LEFT: break;
      default: break;
    }
    static unsigned long lastMovementTime = 0;

    double joyLx = ds_control_data.axisX; double joyLy = ds_control_data.axisY;
    double joyRx = ds_control_data.axisRX; double joyRy = ds_control_data.axisRY;

    bool moveDetected = joyLx > 20 || joyLx < -20 || joyLy > 20 || joyLy < -20 ||
                        joyRx > 20 || joyRx < -20 || joyRy > 20 || joyRy < -20;

    if (moveDetected) {
      lastMovementTime = millis();
      if (currentMode == standbyMode) changeMode(carMode);
    } else {
      unsigned long idleTime = millis() - lastMovementTime;
      if (currentMode == carMode && idleTime >= TIME_TO_STANDBY) {
        changeMode(standbyMode);
      }
    }
  }
  if (currentMode) {currentMode->loop();}
  //Freq debug
  //static uint32_t t0 = micros(); uint32_t now = micros();
  //Serial.println(now - t0); t0 = now;
}
