#include "View.h"

View::View() {
  for (int loop=4; loop <= 11; loop++) {
    pinMode(loop, OUTPUT);
    digitalWrite(loop, HIGH);
  }
  
  this->m_frame = 0;
}
 
/**
  5
6    4
  7
11   9  
  10   (8)
*/
void View::setLedStatus(int mode) {
  for (int loop=4; loop <= 11; loop++) {
    digitalWrite(loop, HIGH);
  }
  
  switch (mode & 0x000F) {
    case Model::eStatic: 
      printStatic();
      break;
    case Model::eSlow:
      printSlow();
      break;
    case Model::eFast: 
      printFast();
      break;      
    case Model::eSetup:
      printSetup();
      break;
  }
  
  if (mode & Model::eUp) {
    digitalWrite(4, LOW);
  } else if (mode & Model::eDown) {
    digitalWrite(6, LOW);
  }
}

void View::printSlow() {
  switch(m_frame) {
    case 1: digitalWrite(10, LOW); break;
    case 2: digitalWrite(10, LOW); break;
    case 3: digitalWrite(7, LOW); break;
    case 4: digitalWrite(7, LOW); break;
    case 5: digitalWrite(5, LOW); break;
    case 6: digitalWrite(5, LOW); break;
    case 7: digitalWrite(7, LOW); break;
    case 8: digitalWrite(7, LOW); break;
  }
}

void View::printFast() {
  switch(m_frame) {
    case 1: digitalWrite(5, LOW); break;
    case 2: digitalWrite(7, LOW); break;
    case 3: digitalWrite(10, LOW); break;
    case 4: digitalWrite(7, LOW); break;
    case 5: digitalWrite(5, LOW); break;
    case 6: digitalWrite(7, LOW); break;
    case 7: digitalWrite(10, LOW); break;
    case 8: digitalWrite(7, LOW); break;
  }
}

void View::printStatic() {
  switch(m_frame) {
    case 1: digitalWrite(7, LOW); break;
    case 3: digitalWrite(7, LOW); break;
    case 5: digitalWrite(7, LOW); break;
    case 7: digitalWrite(7, LOW); break;
  }
}

void View::printSetup() {
  if(m_frame%2 == 0) {
    digitalWrite(11, LOW);
  } else {
    digitalWrite(9, LOW);
  }
}

void View::resetAnim() {
  this->m_frame = 0;
}

void View::nextFrame() {
  this->m_frame = this->m_frame % 8;
  this->m_frame++;
}
