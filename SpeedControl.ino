#include <Servo.h>
#include <EEPROM.h>
#include <aJSON.h>
#include "View.h"
#include "Controller.h"
#include "SerialController.h"
#include "Model.h"

String inputStringHW = "";         // a string to hold incoming data
boolean stringCompleteHW = false;  // whether the string is complete

aJsonStream serial_stream(&Serial);

View* view;
Model* model;
Controller* controller;
SerialController* serial;

void setup() {
  // initialize serial:
  Serial1.begin(4800);
  view = new View();
  model = new Model();
  serial = new SerialController(model, view);
  controller = new Controller(model, view);
  Serial.begin(9600);
  
  // reserve 200 bytes for the inputString:
  inputStringHW.reserve(200);
  attachInterrupt(1, interrupt, CHANGE);
}

void loop() {
  boolean state = digitalRead(2);
  controller->pump(state);
  
  if(stringCompleteHW) {
    serial->serialGet(inputStringHW);
    inputStringHW = "";
    stringCompleteHW = false;
  }
  
  if (serial_stream.available()) {
    /* First, skip any accidental whitespace like newlines. */
    serial_stream.skip();
  }
  
  if (serial_stream.available()) {
    /* Something real on input, let's take a look. */
    aJsonObject *msg = aJson.parse(&serial_stream);
    serial->serialGetUSB(msg);
    aJson.deleteItem(msg);
  }
}

void interrupt() {
  unsigned long eventTime = millis();
  controller->interrupt(eventTime);
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
    inputStringHW += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringCompleteHW = true;
    } 
  }
}
