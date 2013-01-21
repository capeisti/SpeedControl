#pragma once
#include <Arduino.h>
#include "Model.h"
class View {
  public:
    View();
    void setLedStatus(Model::EMode mode);

};
