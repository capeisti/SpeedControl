#include <Servo.h>
#include "View.h"
#include "Controller.h"
#include "Model.h"

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
Servo myservo;

View view;
Model model;
Controller controller(&model, &view);

void setup() {  
  // initialize serial:
//  Serial1.begin(9600);
  Serial.begin(9600);
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  attachInterrupt(1, interrupt, CHANGE);
}

void loop() {
  boolean state = digitalRead(2);
  controller.pump(state);
}

void interrupt() {
  unsigned long eventTime = millis();
  controller.interrupt(eventTime);
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent1() {

  while (Serial1.available()) {
    
    // get the new byte:
    char inChar = (char)Serial1.read();     
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}

