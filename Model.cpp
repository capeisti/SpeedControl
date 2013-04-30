#include "Model.h"
#include <EEPROM.h>

static const int SERVO_MAX = 179;
static const int SERVO_MIN = 1;
static const unsigned int EEPROM_MEM = 0;

Model::Model() {
  this->m_mode = eOff;
  this->m_setup = false;
  this->throttleValue = 0;
  this->speedValue = 0;
  this->lowValue = EEPROM.read(EEPROM_MEM) | EEPROM.read(EEPROM_MEM+1)<<8;
  this->highValue = EEPROM.read(EEPROM_MEM+2) | EEPROM.read(EEPROM_MEM+3)<<8;
  this->targetValue = 0;
  this->servoValue = 0;
  this->servo.attach(12);
}

/**
* Toggle to next mode.
*/
void Model::nextMode() {
  switch (this->m_mode) {
    case eOff: {
      m_mode = eStatic; 
      this->targetValue = this->lowValue;
    } break;
    case eStatic: m_mode = eSlow; break;
    case eSlow: m_mode = eFast; break;
    case eFast: m_mode = eStatic; break;
  }
}

void Model::setMode(EMode mode) {
  m_mode = mode;
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
  
  EEPROM.write(EEPROM_MEM, this->lowValue & 0xFF);
  EEPROM.write(EEPROM_MEM+1, (this->lowValue>>8) & 0xFF);
  EEPROM.write(EEPROM_MEM+2, this->highValue & 0xFF);
  EEPROM.write(EEPROM_MEM+3, (this->highValue>>8) & 0xFF);
}

void Model::setLowValue(int value) {
  this->lowValue = value;
}

void Model::setHighValue(int value) {
  this->highValue = value;
}

void Model::printState() {
  if (Serial) {
    Serial.print("{");
    printJsonField("lowSpeed", this->lowValue, false);
    printJsonField("highSpeed", this->highValue, false);
    printJsonField("targetSpeed", this->targetValue, false);
    printJsonField("measuredSpeed", this->speedValue, false);
    printJsonField("throttleValue", this->throttleValue, false);
    printJsonField("servoValue", this->servoValue, false);
    printJsonField("mode", this->m_mode, true);
    Serial.println("}");
  }
}

void Model::printJsonField(const String& field, int value, boolean lastField) {
  Serial.print("\"");
  Serial.print(field);
  Serial.print("\": \"");
  Serial.print(value);
  Serial.print("\"");
  if (!lastField) {
    Serial.print(", ");
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

/**
* Limit value to be between min and max.
*/
int Model::limit(int value, int min, int max) {
  if (value < min) {
    return min;
  } else if (value > max) {
    return max;
  }
  
  return value;
}

/**
* Called every second.
*/
void Model::secondTick() {
  static int seconds = 0;
  seconds++;
  
  if (!isAutoPilotMode()) {
    seconds = 0;
    return;
  }
  
  if (this->m_mode == eStatic) {
    seconds = 0;
  } else if (this->m_mode == eSlow && seconds > 30) {
    switchTargetValue();  
    seconds = 0;
  } else if (this->m_mode == eFast && seconds > 5) {
    switchTargetValue();
    seconds = 0;
  }
}

void Model::switchTargetValue() {
  if (this->targetValue == this->lowValue) {
    this->targetValue = this->highValue;
  } else {
    this->targetValue = this->lowValue;
  }
}

int Model::doPID() {
    //Do PID
    int e = this->targetValue - this->speedValue;
    static int prevE = e;    
    int p = e;
    static int i = 0;
    i += e;
    int d = e - prevE;
    int pid = p * 10 + i * 5 + d * 2;
    
    prevE = e;
    return pid;
}

boolean Model::isAutoPilotMode() {
  if (this->m_setup || 
      this->m_mode == eOff || 
      this->targetValue == 0) {
    return false;
  }
  
  return true;
}  

/**
* Got speed measurement from plotter.
* Value is knots * 10.
*/
void Model::setMeasuredSpeed(int value) {  
  this->speedValue = value;
  
  //go into PID if in autopilot mode
  if (isAutoPilotMode()) {
    //Input to servo
    int error = doPID();
    setServo(this->servoValue + error); 
  }
}
