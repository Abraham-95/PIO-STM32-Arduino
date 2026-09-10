#include "Arduino.h"
#include "Mode.h"

void ServoMode::init() {
  SerialUSB.println("Servo Mode Initialized");
}

void ServoMode::loop() {
  // Servo mode loop logic
}

void ServoMode::exit() {
  SerialUSB.println("Exiting Servo Mode");
}
