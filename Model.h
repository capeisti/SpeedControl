#pragma once
#include <Arduino.h>

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
    
  private:
    EMode m_mode;
    boolean m_setup;
    int throttleValue;
};
