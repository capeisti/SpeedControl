#include "Model.h"
static const int SERVO_MAX = 179;
static const int SERVO_MIN = 1;

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
}

void Model::printState() {
  if (Serial) {
    Serial.print("lowSpeed=");
    Serial.print(this->lowValue);
    Serial.print(":highSpeed=");
    Serial.print(this->highValue);
    Serial.print(":targetSpeed=");
    Serial.print(this->targetValue);
    Serial.print(":measuredSpeed=");
    Serial.print(this->speedValue);
    Serial.print(":throttleValue=");
    Serial.print(this->throttleValue);
    Serial.print(":servoValue=");
    Serial.print(this->servoValue);
    Serial.print(":mode=");
    Serial.print(this->m_mode);
    Serial.println("");
  }
}

/**
* Set throttle potentiometer value.
*/
void Model::setThrottle(int value) {
  this->throttleValue = value;
  this->m_mode = eOff;
  setServo(this->throttleValue);
}

/**
* Return throttle potentiometer value.
*/
int Model::getThrottle() {
  return throttleValue;
}

/**
* Got servo setup value from PID or throttle.
* Value is between 0..1024.
*/
void Model::setServo(int value) {
  this->servoValue = limit(value, 0, 1023);
  int mapped = map(this->servoValue, 0, 1023, SERVO_MIN, SERVO_MAX);
  this->servo.write(mapped);
}

int Model::limit(int value, int min, int max) {
  if (value < min) {
    return min;
  } else if (value > max) {
    return max;
  }
  
  return value;
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
    
    //Input to servo
    setServo(this->servoValue + error); 
  }
}
