#include "Model.h"

Model::Model() {
  this->m_mode = eOff;
  this->m_setup = false;
  this->throttleValue = 0;
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
  }
}

int Model::getThrottle() {
  return throttleValue;
}
