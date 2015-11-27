#include "NanoMouseMotors.h"
#include "NanoMouseSensors.h"

NanoMouseMotors motors;
NanoMouseSensors<4,A7, 3,A6, 2,A5> sensors; // <lE,lD, fE,fD, rE,rD>

const byte ledPin = 13;
const byte buttonPin = 9;

void setup() {
  
  motors.attach(6,5);

  Serial.begin(9600);
  sensors.configure();
  
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  while(digitalRead(buttonPin)){}
  delay(1000);
  
  //forwardTime(5000);
  //turn(LEFT,180);
  //turn(RIGHT,180);
  //motors.polygon(2000, 4);
}

void loop() {
  sensors.view();
  delay(100);
  //digitalWrite(ledPin, HIGH);
}
