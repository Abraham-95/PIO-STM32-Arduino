
#pragma once
#include <Arduino.h>

class Mode {
  public:
  virtual const char* getModeName() const = 0;
    virtual void init() = 0;
    virtual void loop() = 0;
    virtual void exit() = 0;
};

class StandbyMode : public Mode {
  public:
    const char* getModeName() const override { return "Standby Mode"; }
    void loop() override;
    void init() override;
    void exit() override;
};

class CarMode : public Mode {
  public:
    const char* getModeName() const override { return "Car Mode"; }
    void loop() override;
    void init() override;
    void exit() override;
};

class ServoMode : public Mode {
  public:
    const char* getModeName() const override { return "Servo Mode"; }
    void loop() override;
    void init() override;
    void exit() override;
};

class SpeedMode : public Mode {
  public:
    const char* getModeName() const override { return "Speed Mode"; }
    void loop() override;
    void init() override;
    void exit() override;
};

extern Mode *currentMode;
extern Mode *previousMode;

extern StandbyMode *standbyMode;
extern CarMode *carMode;
extern ServoMode *servoMode;
extern SpeedMode *speedMode;

const char* modeToString(Mode* Mode);
void changeMode(Mode* next);
extern uint8_t getModeCode();
