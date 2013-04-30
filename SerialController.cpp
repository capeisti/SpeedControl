#include "SerialController.h"

SerialController::SerialController(Model* model, View* view) {
  this->m_model = model;
  this->m_view = view;
}

/**
* Parse field from NMEA-0183 line.
*/
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
    m_model->setMeasuredSpeed(atoi(buf));
    m_view->setCommAlive(); 
  }
  
  Serial1.print(line);
}

void SerialController::serialGetUSB(const String& line) {
  Serial.println("got something");
}
