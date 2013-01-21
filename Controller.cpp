#include "Controller.h"

Controller::Controller(Model* model, View* view) {
  this->m_model = model;
  this->m_view = view;
  this->eventTime = 0;
  this->pressTime = 0;
  pinMode(2, INPUT);
  
  m_view->setLedStatus(m_model->getMode());
}

void Controller::keyEvent(boolean pressed) {
  
  if (pressed) {
    this->pressTime = millis();
  }
  
  if (!pressed) {
    this->pressTime = 0;
    m_model->nextMode();
    m_view->setLedStatus(m_model->getMode());
  }
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
  if (eventTime != 0 && millis() - eventTime > 50) {    
    keyEvent(pin2State);
    eventTime = 0;
  }
  
  if (this->pressTime != 0 &&
        this->m_model->getMode() != Model::eSetup &&         
        millis() - this->pressTime > 1000) {
    this->m_model->setup();
    m_view->setLedStatus(m_model->getMode());
  }
}
