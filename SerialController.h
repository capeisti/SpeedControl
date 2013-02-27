#pragma once
#include <Arduino.h>
#include "Model.h"

class SerialController {
  public:
    SerialController(Model* model);
    void serialGet(const String& line);
    
  private:
    String getField(const String& line, int field);
    Model* m_model;
};
