#pragma once
#include <Arduino.h>
#include "Model.h"
#include "View.h"

class Controller {
  public:
    Controller(Model* model, View* view);  
    void interrupt(unsigned long time);
    void pump(boolean pin2State);
    
  private: 
    void handleButtonEvent(boolean pin2State);
    void handleKeepPressed();
    void handleViewUpdate();
    void handlePotentiometer();
   
    Model* m_model;
    View* m_view;
    volatile unsigned long eventTime;
    unsigned long pressTime;
};
