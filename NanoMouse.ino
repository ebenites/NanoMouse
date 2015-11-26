#include "NanoMouseMotors.h"

NanoMouseMotors motors;

const byte ledPin = 13;
const byte buttonPin = 9;

void setup() {
  
  motors.attach(6,5);
  
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  while(digitalRead(buttonPin)){}
  delay(1000);
  
  //forwardTime(5000);
  //turn(LEFT,180);
  //turn(RIGHT,180);
  motors.polygon(2000, 4);
}

void loop() {
  digitalWrite(ledPin, HIGH);
}
