#pragma once
#include <Arduino.h>
#include <Servo.h>

class Model {
  public:
    enum EMode {
      eOff = 0,
      eSetup = 1,
      eStatic = 2, 
      eSlow = 4, 
      eFast = 8,
      eUp = 16,
      eDown = 32      
    };
    Model();
    void nextMode();
    void setup();
    void resume();
    EMode getMode();
    void pump();
    int getThrottle();
    
    void setServo(int value);
    void setSpeed(int value);
    
  private:
    EMode m_mode;
    boolean m_setup;
    int throttleValue;
    int speedValue;
    Servo servo;
};
