#pragma once
#include <Arduino.h>
#include "Model.h"
#include "View.h"

class SerialController {
  public:
    SerialController(Model*, View*);
    void serialGet(const String& line);
    
  private:
    String getField(const String& line, int field);
    Model* m_model;
    View* m_view;
};
