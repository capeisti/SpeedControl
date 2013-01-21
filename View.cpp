#include "View.h"

View::View() {
  for (int loop=4; loop <= 11; loop++) {
    pinMode(loop, OUTPUT);
    digitalWrite(loop, HIGH);
  }
}
  
void View::setLedStatus(Model::EMode mode) {
  for (int loop=4; loop <= 11; loop++) {
    digitalWrite(loop, HIGH);
  }
  
  switch(mode) {
    case Model::eStatic: 
      digitalWrite(4, LOW);
      break;
    case Model::eSlow:
      digitalWrite(6, LOW);
      break;
    case Model::eFast: 
      digitalWrite(8, LOW);
      break;
      
    case Model::eSetup:
      digitalWrite(10, LOW);
      break;
  }
}
