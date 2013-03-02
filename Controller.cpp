#include "Controller.h"

Controller::Controller(Model* model, View* view) {
  this->m_model = model;
  this->m_view = view;
  this->eventTime = 0;
  this->pressTime = 0;
  pinMode(2, INPUT);
  
  m_view->setLedStatus(m_model->getMode());
}

/**
  Handle key event for pressed and released.
*/
void Controller::keyEvent(boolean pressed) {
  //On press down start measure time pressed
  if (pressed) {
    this->pressTime = millis();
  } else { //on release change mode
    this->pressTime = 0;
    
    //Return from setup to normal mode
    if (this->m_model->getMode() == Model::eSetup) {
      m_model->resume();
    } else { //select next mode
      m_model->nextMode();
    }
  }
  
  m_view->resetAnim();
}

/**
  Pin 2 interrupt handler.
*/
void Controller::interrupt(unsigned long time) {
  this->eventTime = time;
}

/**
  pump() is called on every Arduino loop().
  Parameter pin2State is a speed hack.
*/
void Controller::pump(boolean pin2State) {
  //After 50ms of last interrupt assume that pin 2 is settled
  if (eventTime != 0 && millis() - eventTime > 50) {    
    keyEvent(pin2State);
    eventTime = 0;
  }
  
  //if pressed down for over second and not yet in setup mode
  if (this->pressTime != 0 &&
        this->m_model->getMode() != Model::eSetup &&         
        millis() - this->pressTime > 1000) {
    //then go to setup mode
    this->m_model->setup();    
  }
  
  //Update view in every 300 ms.
  static unsigned long frameTime = 0;
  if (millis() - frameTime > 300) {
    static int prevThrottle = m_model->getThrottle();
  
    int mode = m_model->getMode();
    if (prevThrottle != m_model->getThrottle()) {
      mode |= (m_model->getThrottle()-prevThrottle) > 0?Model::eUp:Model::eDown;
      prevThrottle = m_model->getThrottle();
    }
  
    m_view->nextFrame();
    frameTime = millis();
    m_view->setLedStatus(mode);
  }
  
  //Print state for every second.
  static unsigned long secondTime = 0;
  if (millis() - secondTime > 1000) {
    m_model->printState();
    secondTime = millis();
  }
  
  //Throttle potentiometer.
  //Calculate mean over 1024 samples and have treshold value to avoid instability.
  static long sum = 0;
  static int counter = 0;
  sum += analogRead(4); 
  counter++;
  
  if (counter == 1023) {
    int meanValue = sum>>10;
    sum = 0;
    counter = 0;
    
    int current = m_model->getThrottle();
    //Update only if value has changed over treshold.
    if (abs(current-meanValue) > 3) {
      m_model->setThrottle(meanValue);
    }  
  }
}
