#include "Arduino.h"
#include "Mode.h"

void CarMode::init() {
  SerialUSB.println("Car Mode Initialized");
}

void CarMode::loop() {
  // Car mode loop logic
}

void CarMode::exit() {
  SerialUSB.println("Exiting Car Mode");
}
