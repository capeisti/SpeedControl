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
    int getThrottle();
    void setThrottle(int value);
    
    void setMeasuredSpeed(int value);
    
  private:
  void setServo(int value);
  
    EMode m_mode;
    boolean m_setup;
    int throttleValue;
    int servoValue;
    int speedValue;
    int targetValue;
    Servo servo;
    int lowValue;
    int highValue;
};
