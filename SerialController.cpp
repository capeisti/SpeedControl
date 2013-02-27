#include "SerialController.h"

SerialController::SerialController(Model* model) {
  this->m_model = model;
}

String SerialController::getField(const String& line, int field) {
  int index = 0;
  for (int loop=0; loop < field; loop++) {
    index = line.indexOf(",", index);
    if(index == -1) {
      return "";
    }
    
    index++;
  }
  
  int endIndex = line.indexOf(",", index);
  
  if (endIndex == -1) {
    return line.substring(index);
  }
  
  return line.substring(index, endIndex);
}

/**
* handle serial data from plotter.
*/
void SerialController::serialGet(const String& line) {
  if (line.startsWith("$GPRMC")) {    
    String speed = getField(line, 7);
    speed.replace(".", "");
    char buf[speed.length()+1];
    speed.toCharArray(buf, sizeof(buf));
    m_model->setSpeed(atoi(buf)); 
  }
}
