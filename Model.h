#pragma once
#include <Arduino.h>

class Model {
  public:
    enum EMode {eStatic, eSlow, eFast, eSetup};
    Model();
    void nextMode();
    void setup();
    EMode getMode();
    
  private:
    EMode m_mode;
};
