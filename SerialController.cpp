#include <aJSON.h>
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

void SerialController::serialGetUSB(aJsonObject *json) {
  aJsonObject* lowSpeed = aJson.getObjectItem(json, "lowSpeed");
  aJsonObject* highSpeed = aJson.getObjectItem(json, "highSpeed");
  aJsonObject* throttleValue = aJson.getObjectItem(json, "throttleValue");
  aJsonObject* mode = aJson.getObjectItem(json, "mode");
  
  if (lowSpeed) {
    m_model->setLowValue(atoi(lowSpeed->valuestring));
  }
  
  if (highSpeed) {
    m_model->setHighValue(atoi(highSpeed->valuestring));
  }
  
  if (throttleValue) {
    m_model->setThrottle(atoi(throttleValue->valuestring));
  }
  
  if (mode) {
    m_model->setMode((Model::EMode)atoi(mode->valuestring));
  }
}
