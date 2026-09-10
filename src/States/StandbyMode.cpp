#include "Arduino.h"
#include "Mode.h"

void StandbyMode::init() {
  SerialUSB.println("Standby Mode Initialized");
}

void StandbyMode::loop() {
  // Standby mode loop logic
}

void StandbyMode::exit() {
  SerialUSB.println("Exiting Standby Mode");
}
