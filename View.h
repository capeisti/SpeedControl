#pragma once
#include <Arduino.h>
#include "Model.h"
class View {
  public:
    View();
    void setLedStatus(int mode);
    void setCommAlive();
    void nextFrame();
    void resetAnim();

  private:
    void printFast();
    void printSlow();
    void printStatic();
    void printSetup();
    int m_frame;
};
