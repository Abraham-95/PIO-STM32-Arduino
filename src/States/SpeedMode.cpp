#include "Arduino.h"
#include "Mode.h"

void SpeedMode::init() {
  SerialUSB.println("Speed Mode Initialized");
}

void SpeedMode::loop() {
  // Speed mode loop logic
}

void SpeedMode::exit() {
  SerialUSB.println("Exiting Speed Mode");
}
