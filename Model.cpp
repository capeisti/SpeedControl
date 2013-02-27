#include "Model.h"
Model::Model() {
  this->m_mode = eOff;
  this->m_setup = false;
  this->throttleValue = 0;
  this->speedValue = 0;
  this->servo.attach(12);
}

void Model::nextMode() {
  switch (this->m_mode) {
    case eOff: m_mode = eStatic; break;
    case eStatic: m_mode = eSlow; break;
    case eSlow: m_mode = eFast; break;
    case eFast: m_mode = eStatic; break;
  }
}

Model::EMode Model::getMode() {
  if (m_setup) {
    return eSetup;
  }
  
  return m_mode;
}

void Model::setup() {
  this->m_setup = true;
}


void Model::resume() {
  this->m_setup = false;
}

void Model::pump() {
  int value = analogRead(4);
  if (abs(throttleValue-value) > 5) {
    throttleValue = value;
    m_mode = eOff;
    
    setServo(value);
  }
}

int Model::getThrottle() {
  return throttleValue;
}

void Model::setServo(int value) {
  int mapped = map(value, 0, 1023, 0, 179);
  this->servo.write(mapped);
  Serial.println(mapped);
}

void Model::setSpeed(int value) {
  this->speedValue = value;
  Serial.println(value);
}
