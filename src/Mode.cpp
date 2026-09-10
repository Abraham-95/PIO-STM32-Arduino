#include "Mode.h"

void changeMode(Mode* next) {
  if (next == nullptr || next == currentMode) return;
  if (currentMode) currentMode->exit();
  previousMode = currentMode;
  currentMode = next;
  currentMode->init();
}

const char* modeToString(Mode* Mode) {
  if(Mode == standbyMode)     return "STANDBY MODE";
  if(Mode == carMode)         return "CAR MODE";
  if(Mode == servoMode)       return "SERVO MODE";
  if(Mode == speedMode)       return "SPEED MODE";
  return "UNKNOWN";
}

uint8_t getModeCode() {
  if(currentMode == standbyMode) return 0;
  if(currentMode == carMode)     return 1;
  if(currentMode == servoMode)   return 2;
  if(currentMode == speedMode)   return 3;
  return 255;
}
