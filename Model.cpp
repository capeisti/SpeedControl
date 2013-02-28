#include "Model.h"
Model::Model() {
  this->m_mode = eOff;
  this->m_setup = false;
  this->throttleValue = 0;
  this->speedValue = 0;
  this->lowValue = 0;
  this->highValue = 0;
  this->targetValue = 0;
  this->servoValue = 0;
  this->servo.attach(12);
}

/**
* Toggle to next mode.
*/
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

/**
* Go to setup mode.
*/
void Model::setup() {
  this->m_setup = true;
  this->lowValue = this->speedValue;
}

/**
* Resume from setup mode.
*/
void Model::resume() {
  this->m_setup = false;
  if (this->speedValue < this->lowValue) {
    this->highValue = this->lowValue;
    this->lowValue = this->speedValue;
  } else {
    this->highValue = this->speedValue;
  }
  
  Serial.print("low: ");
  Serial.print(this->lowValue);
  Serial.print(" high: ");
  Serial.println(this->highValue);
}

/**
* System loop.
*/
void Model::pump() {
  static long sum = 0;
  static int counter = 0;
  sum += analogRead(4); 
  counter++;
  
  if (counter == 1023) {
    int meanValue = sum>>10;
    sum = 0;
    counter = 0;
    
    if (abs(throttleValue-meanValue) > 3) {
      this->throttleValue = meanValue;
      this->m_mode = eOff;    
      setThrottle(this->throttleValue);      
    }  
  }
}

/**
* Return throttle potentiometer value.
*/
int Model::getThrottle() {
  return throttleValue;
}

/**
* Got throttle potentiometer value from user or PID.
* Value is between 0..1024.
*/
void Model::setThrottle(int value) {
  this->servoValue = value;
  int mapped = map(value, 0, 1023, 2, 179);
  this->servo.write(mapped);
}

/**
* Got speed measurement from plotter.
* Value is knots * 10.
*/
void Model::setMeasuredSpeed(int value) {
  this->targetValue = this->lowValue; 
  this->speedValue = value;
  
  //go into PID if in autopilot mode
  if (!this->m_setup && this->m_mode != eOff) {
    //Drop autopilot if not target speed
    if (this->targetValue == 0) {
      this->m_mode = eOff;
      return;
    }
    
    //Do PID
    int p = this->targetValue - this->speedValue;
    int i = 0;
    int d = 0;
    int error = p * 10 + i + d;
    this->servoValue += error;
    
    //Input to servo
    setThrottle(this->servoValue); 
  }
}
